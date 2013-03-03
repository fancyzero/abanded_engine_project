#include "stdafx.h"
#include "renderingthread.h"
#include "core.h"
#include "engine.h"
/////some internal plugins
#include "plugins\inputdevice\inputdevice_plugin.h"
#include "plugins\inputdevice\inputdevice.h"


namespace mania
{
static core* g_pCore = NULL;

core* core::get_instance()
{
	if ( g_pCore != NULL )
		return g_pCore;
	else
		return ( g_pCore = new core() );
}

MANIA_ENGINE_API core* get_core()
{
	return core::get_instance();
}

void core::DestroyInstance()
{
	if ( g_pCore != NULL )
		delete g_pCore;
	g_pCore = NULL;
}

core::core( void )
		: m_rendering_thread( NULL )
		, m_renderingthread_disabled( FALSE )
		, m_renderdevice_plugin( NULL )
{
}

core::~core( void )
{
	uninit();
}

DWORD core::init( HINSTANCE hInst, HWND hWnd )
{
	m_hWnd = hWnd;
	m_hInstance = hInst;
	main_thread_lock.create( TRUE, FALSE, NULL );
	render_thread_lock.create( TRUE, FALSE, NULL );
	string value;
	m_scriptvm.initialize();
	if ( initialize_plugins() == grc_failed )
	{
		get_log()->write( 100, 0xff0000, _T( "Initialize plugins failed.\n" ) );
	}
	if ( get_cmdline_param( _T( "disable_renderingthread" ), value ) )
		m_renderingthread_disabled = TRUE;
	if ( BeginRenderingThread() == FALSE )
		return grc_failed;
	return grc_succeeded;
}

DWORD core::uninit()
{
	if ( EndRenderingThread() == FALSE )
		return grc_failed;
	return grc_succeeded;
}

BOOL core::get_cmdline_param( const string& paramname, string& value )
{
	std::vector< std::pair<string, string> >::iterator it;
	for ( it = m_cmdline_params.begin(); it != m_cmdline_params.end(); ++it )
	{
		if ( paramname == ( *it ).first )
		{
			value = ( *it ).second;
			return TRUE;
		}
	}
	return FALSE;
}

plugin* core::get_plugin( eplugintype type )
{
	switch ( type )
	{
	case plugintype_renderer:
		return m_renderdevice_plugin;
	default:
		return NULL;
	}
}

BOOL core::BeginRenderingThread()
{
	m_rendering_thread = new rendering_thread();
	if ( renderingthread_disabled() )
		m_rendering_thread->Init( m_hWnd );
	else
		m_rendering_thread->Begin(( void* )m_hWnd, FALSE );
	return TRUE;
}

BOOL core::EndRenderingThread()
{
	if ( m_rendering_thread == NULL )
		return TRUE;
	BOOL bRet = m_rendering_thread->KillSoftly( INFINITE );
	delete m_rendering_thread;
	return bRet;
}

void core::BeginFrame()
{
	for ( UINT i = 0; i  < m_Components.size(); i++ )
	{
		if ( m_Components[i]->OnBeginFrame() != ccrc_continue )
			break;
	}
	{
		class begin_frame_task : public render_task
		{
		public:
			begin_frame_task( rendering_thread* pRenderingThread )
					: render_task( pRenderingThread )
			{
			}
			DWORD execute()
			{
				m_rendering_thread->get_renderdevice()->begin_frame();
				return grc_succeeded;
			}
		};
		begin_frame_task* newtask = new begin_frame_task( mania::get_core()->get_rendering_thread() );
		mania::get_core()->get_rendering_thread()->AddRenderTask( newtask );
	}
}

void core::update()
{
	for ( UINT i = 0; i  < m_Components.size(); i++ )
	{
		if ( m_Components[i]->OnUpdate() != ccrc_continue )
			break;
	}
}

void core::EndFrame()
{
	for ( UINT i = 0; i  < m_Components.size(); i++ )
	{
		if ( m_Components[i]->OnEndFrame() != ccrc_continue )
			break;
	}
	ENQUEUE_RENDER_TASK_BEGIN( end_frame_task )
	m_rendering_thread->get_renderdevice()->end_frame();
	return grc_succeeded;
	ENQUEUE_RENDER_TASK_END( end_frame_task );
}

void core::BeginVisualFrame()
{
	for ( UINT i = 0; i  < m_Components.size(); i++ )
	{
		if ( m_Components[i]->OnBeginVisualFrame() != ccrc_continue )
			break;
	}
}

void core::UpdateVisual()
{
	for ( UINT i = 0; i  < m_Components.size(); i++ )
	{
		if ( m_Components[i]->OnUpdateVisual() != ccrc_continue )
			break;
	}
}

void core::EndVisualFrame()
{
	for ( UINT i = 0; i  < m_Components.size(); i++ )
	{
		if ( m_Components[i]->OnEndVisualFrame() != ccrc_continue )
			break;
	}
}

void core::BeginLogicalFrame()
{
	for ( UINT i = 0; i  < m_Components.size(); i++ )
	{
		if ( m_Components[i]->OnBeginLogicalFrame() != ccrc_continue )
			break;
	}
}

void core::UpdateLogical()
{
	for ( UINT i = 0; i  < m_Components.size(); i++ )
	{
		if ( m_Components[i]->OnUpdateLogical() != ccrc_continue )
			break;
	}
}

void core::EndLogicalFrame()
{
	for ( UINT i = 0; i  < m_Components.size(); i++ )
	{
		if ( m_Components[i]->OnEndLogicalFrame() != ccrc_continue )
			break;
	}
}

void core::parse_cmdline( const TCHAR* cmdline )
{
	const TCHAR* i = cmdline;
	const TCHAR* ts = 0;//token start
	const TCHAR* vs = 0;//value start
	int tlen = 0;//token len
	int vlen = 0;//value len
	const TCHAR* judgei = 0;
	enum eparsestate
	{
		s_lookfor_token,
		s_read_token,
		s_lookfor_value,
		s_read_value,
		s_save
	};
	eparsestate state = s_lookfor_token;
	do
	{
		switch ( state )
		{
		case s_lookfor_token:
		{
			if ( *i == _T( '-' ) )
			{
				state = s_read_token;
				ts = i + 1;
			}
		}
		break;
		case s_read_token:
		{
			if ( *i == _T( ' ' ) )
			{
				state = s_lookfor_value;
				break;
			}
			if ( *i == _T( '=' ) )
			{
				vs = i + 1;
				state = s_read_value;
			}
			if ( *i == 0 )
			{
				state = s_save;
				break;
			}
			tlen++;
		}
		break;
		case s_lookfor_value:
		{
			if ( *i == _T( '-' ) )
			{
				state = s_save;
				break;
			}
			if ( *i == _T( '=' ) )
			{
				vs = i + 1;
				state = s_read_value;
				break;
			}
			if ( *i != _T( ' ' ) )
			{
				vs = i;
				state = s_read_value;
				break;
			}
		}
		break;
		case s_read_value:
		{
			if ( *i == _T( '-' ) || *i == 0 )
			{
				state = s_save;
				break;
			}
			else
				vlen++;
		}
		break;
		case s_save:
		{
			string strtoken;
			string strvalue;
			if ( ts )
			{
				strtoken = ts;
				strtoken = strtoken.substr( 0, tlen );
			}
			if ( vs )
			{
				strvalue = vs;
				strvalue = strvalue.substr( 0, vlen );
			}
			std::pair<string, string> p;
			p.first = strtoken;
			p.second = strvalue;
			m_cmdline_params.push_back( p );
			state = s_lookfor_token;
			ts = 0;
			vs = 0;
			tlen = 0;
			vlen = 0;
		}
		break;
		}
		judgei = i;
		if ( state != s_save )
			i++;
		else
		{
			i--;
			judgei = i;
		}
	} while ( *judgei );
}


//! functions for plugins

//! load plugin dll by type
plugin*	core::load_plugin( eplugintype type )
{
	string	plugin_key;
	TCHAR	filename[MAX_PATH+1];
	switch ( type )
	{
	case plugintype_renderer:
		plugin_key = _T( "renderdevice" );
		break;
	case plugintype_inputdevice:
		return new inputdevice_plugin_impl();
		break;
	default:
		break;
	}
	DWORD ret = GetPrivateProfileString( _T( "plugins" ), plugin_key.c_str(), _T( "" ),
	                                     filename, MAX_PATH, _T( ".\\mania_engine.ini" ) );
	if ( ret == 0 )
		return NULL;
	HMODULE module = LoadLibrary( filename );
	if ( module == NULL )
		return NULL;
	typedef mania::plugin*( CALLBACK* CREATE_PLUGIN_PROC )();
	CREATE_PLUGIN_PROC proc = ( CREATE_PLUGIN_PROC )GetProcAddress( module, ( "create_plugin" ) );
	return proc();
}

//! load and initalize all plugins
DWORD core::initialize_plugins()
{
	//!load render plugin
	m_renderdevice_plugin = ( renderdevice_plugin* )load_plugin( plugintype_renderer );
	if ( m_renderdevice_plugin == NULL )
		return grc_failed;
	m_inputdevice_plugin = ( inputdevice_plugin* )load_plugin( plugintype_inputdevice );
	if ( m_inputdevice_plugin == NULL )
		return grc_failed;
	return grc_succeeded;
}
//script------------
BOOL core::execute_script( const string& script )
{
	m_scriptvm.execute_script( script.c_str() );
	
	return TRUE;
}
}