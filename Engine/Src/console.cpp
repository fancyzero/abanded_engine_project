#include "StdAfx.h"
#include "console.h"
#include <io.h>
#include <fcntl.h>     /* for _O_TEXT and _O_BINARY */
#include "atlbase.h"
#include "atlapp.h"
#include "atlctrls.h"
#include <winuser.h>
namespace mania
{
class console_input_thread : public Thread
{
public:
	console_input_thread() {};
	~console_input_thread() {};

	virtual DWORD OnBegin( void* userparam );
	virtual DWORD OnLoop( void* userparam );
	console*	m_console;
};

//////////////////////
//output window
//////////////////////
class input_window : public CWindowImpl<input_window>
{
public:
	BEGIN_MSG_MAP( console_window )
	MESSAGE_HANDLER( WM_KEYDOWN, on_keydown )
	END_MSG_MAP()

	input_window()
	{
		m_history_pos = 0;
	}
	void add_cmd( TCHAR* cmd )
	{
		if ( (!m_cmd_history.empty()) && (m_cmd_history.back() == cmd) )
			return;
		m_cmd_history.push_back(cmd);
		m_history_pos = m_cmd_history.size()-1;
	}

	void history_up()
	{
		if ( m_cmd_history.empty() )
			return;
		recall_history();		
		if (m_history_pos != 0 )
			m_history_pos--;		
	}
	void history_down()
	{
		if ( m_cmd_history.empty() )
			return;
		recall_history();
		if ( m_history_pos < m_cmd_history.size()-1 )	
			m_history_pos++;		
	}

	void recall_history()
	{
		SetWindowText( m_cmd_history[m_history_pos].c_str() );
	}

	LRESULT on_keydown( UINT /*uMsg*/, WPARAM wp, LPARAM /*lParam*/, BOOL& bHandled )
	{
		bHandled = FALSE;
		if ( wp == VK_RETURN )
		{
			int cnt = GetWindowTextLength();
			TCHAR* text = new TCHAR[cnt+1];
			GetWindowText( text, cnt + 1 );
			get_core()->execute_script( text );
			add_cmd( text );
			SetWindowText( NULL );
			delete []text;
			bHandled = TRUE;			
		}
		if ( wp == VK_UP )
			history_up();
		if ( wp == VK_DOWN )
			history_down();
		return 0;
	}
	std::vector<string>				m_cmd_history;
	std::size_t						m_history_pos;
};
class console_window : public CWindowImpl<console_window>
{
public:
	BEGIN_MSG_MAP( console_window )
	MESSAGE_HANDLER( WM_CREATE, on_create )
	MESSAGE_HANDLER( WM_SIZE, on_size )
	END_MSG_MAP()

	console_window()
			: richedit_module( null )
	{
		richedit_module = LoadLibrary( output_edit.GetLibraryName() );
	}
	~console_window()
	{
		if ( richedit_module )
			FreeLibrary( richedit_module );
	}

	LRESULT on_create( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
	{
		CMenuHandle mnu = this->GetSystemMenu( FALSE );
		mnu.EnableMenuItem( SC_CLOSE, MF_BYCOMMAND | MF_GRAYED );
		RECT client_rect;
		GetClientRect( &client_rect );
		output_edit.Create( m_hWnd, &client_rect, null, ES_READONLY | ES_MULTILINE | WS_VSCROLL | WS_HSCROLL |  WS_VISIBLE | WS_CHILD, WS_EX_CLIENTEDGE );
		output_font.CreateFont(
		    12,                        // nHeight
		    0,                         // nWidth
		    0,                         // nEscapement
		    0,                         // nOrientation
		    FW_NORMAL,                 // nWeight
		    FALSE,						// bItalic
		    FALSE,                     // bUnderline
		    0,                         // cStrikeOut
		    DEFAULT_CHARSET,              // nCharSet
		    OUT_DEFAULT_PRECIS,        // nOutPrecision
		    CLIP_DEFAULT_PRECIS,       // nClipPrecision
		    DEFAULT_QUALITY,           // nQuality
		    DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		    _T( "Bitstream Vera Sanst" ) );
		output_edit.SetFont( output_font );
		input_edit.Create( m_hWnd, &client_rect, null, WS_VISIBLE | WS_CHILD, WS_EX_CLIENTEDGE, 0x88 );
		input_edit.SetFont( output_font );
		iw.SubclassWindow( input_edit );
		//input_edit.SetWindowLongPtr( GWLP_WNDPROC, (LONG_PTR)input_edit_proc );
		resize();
		return 0;
	}

	LRESULT on_size( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
	{
		resize();
		return 0;
	}

	void resize()
	{
		RECT rc;
		GetClientRect( &rc );
		int height = 24;
		rc.bottom -= height;
		output_edit.SetWindowPos( null, rc.left, rc.top, rc.right, rc.bottom, 0 );
		GetClientRect( &rc );
		rc.top = rc.bottom - height + 1;
		input_edit.SetWindowPos( null, rc.left, rc.top, rc.right, rc.bottom, 0 );
	}

	CFont			output_font;
	CRichEditCtrl	output_edit;
	CEdit			input_edit;
	input_window	iw;
	HMODULE			richedit_module;
};

class console_output_thread : public Thread
{
public:
	console_output_thread();
	~console_output_thread() {};

	virtual DWORD OnBegin( void* userparam );
	virtual DWORD OnLoop( void* userparam );
	void print( DWORD prop, TCHAR* text );

	class console_pipe_thread*		m_stdout_pipe;
	class console_pipe_thread*		m_stderr_pipe;
	console_window			m_window;
	console*				m_console;
};

class console_pipe_thread : public Thread
{
public:
	console_pipe_thread( console_output_thread* host_thread )
		:m_output_thread(host_thread)
	{};
	~console_pipe_thread(){};

	virtual DWORD OnBegin( void* userparam );
	virtual DWORD OnLoop( void* userparam );
	
	struct pipehandle
	{
		HANDLE				m_readtmp;
		HANDLE				m_write;
	};
	pipehandle				m_pipe;
	console_output_thread*	m_output_thread;
};

DWORD console_pipe_thread::OnBegin( void* userparam )
{
	FILE* f = (FILE*)(userparam);
	if ( !CreatePipe( &m_pipe.m_readtmp, &m_pipe.m_write, 0, 0 ) )
		get_log()->write(1, 0xff0000, mania::get_string(L"CONSOLE_CREATE_PIPE_FAILED").c_str());
	int hCrt;
	FILE *hf;
	hCrt = _open_osfhandle(( long )m_pipe.m_write,_O_WTEXT);
	hf = _wfdopen( hCrt, L"w" );
	*f = *hf;
	int i = setvbuf( f, NULL, _IONBF, 0 );
	return grc_succeeded;
}

DWORD console_pipe_thread::OnLoop( void* userparam )
{
	FILE* f = *(FILE**)(userparam);
	const unsigned int buff_maxsize = 1024;
	TCHAR buffer[buff_maxsize];
	DWORD nBytesRead;
	if ( !ReadFile( m_pipe.m_readtmp, buffer, ( buff_maxsize - 1 )*sizeof( TCHAR ),
	                &nBytesRead, NULL ) || !nBytesRead )
	{
		if ( GetLastError() == ERROR_BROKEN_PIPE )
			return grc_succeeded; // pipe done - normal exit path.
		else
			GetTickCount();
	}
	nBytesRead /= sizeof( TCHAR );
	if (( nBytesRead > 0 ) && ( nBytesRead < buff_maxsize ) )
	{
		buffer[nBytesRead] = 0;
		m_output_thread->print( 0, buffer );
	}
	return grc_succeeded;
}



console::console( void )
		: m_input_thread( null )
{
}

console::~console( void )
{
	m_input_thread->KillSoftly( 1000 );
	m_output_thread->KillSoftly( 1000 );
	delete m_input_thread;
	delete m_output_thread;
}

BOOL console::init()
{
	start_input_thread();
	start_output_thread();
	return TRUE;
}

void console::write( TCHAR* str )
{
}

void console::clear()
{
}

BOOL console::start_input_thread()
{
	m_input_thread = new console_input_thread();
	m_input_thread->Begin( this, FALSE );
	return TRUE;
}

BOOL console::start_output_thread()
{
	m_output_thread = new console_output_thread();
	m_output_thread->Begin( this, FALSE );
	return TRUE;
}

console* console::create()
{
	return new console();
}

//////////////////////
//console input thread
//////////////////////
DWORD console_input_thread::OnBegin( void* userparam )
{
	m_console = ( console* )userparam;
	return grc_succeeded;
}

DWORD console_input_thread::OnLoop( void* userparam )
{
	Sleep( 0 );
	return grc_succeeded;
}


//////////////////////
//console output thread
//////////////////////
console_output_thread::console_output_thread( void )
{
	RECT rc;
	rc.left = rc.top = 0;
	rc.bottom = 512;
	rc.right = 512;
	m_stdout_pipe = m_stderr_pipe = NULL;
	m_window.Create( NULL, &rc, L"Mania::Console", WS_VISIBLE | WS_OVERLAPPEDWINDOW );
	m_stdout_pipe = new console_pipe_thread( this );
	m_stdout_pipe->Begin( stdout, FALSE );
	m_stderr_pipe = new console_pipe_thread( this );
	m_stderr_pipe->Begin( stderr, FALSE );
};
void console_output_thread::print( DWORD prop, TCHAR* text )
{
	m_window.output_edit.AppendText( text );
}

DWORD console_output_thread::OnBegin( void* userparam )
{
	m_console = ( console* )userparam;
	return grc_succeeded;
}

DWORD console_output_thread::OnLoop( void* userparam )
{
	if ( m_stdout_pipe )
		m_stdout_pipe->wait(0xFFFFFFFF);
	if ( m_stderr_pipe )
		m_stderr_pipe->wait(0xFFFFFFFF);	
	return grc_succeeded;
}

}