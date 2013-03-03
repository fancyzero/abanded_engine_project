#pragma once

namespace mania
{
class rendering_thread;
class log;
class timer;
enum core_componentReturnCode
{
	ccrc_continue = 0,
	ccrc_break,
};

class core_component
{
public:
	virtual DWORD OnBeginFrame()		{	return ccrc_continue;	}
	virtual DWORD OnUpdate()			{	return ccrc_continue;	}
	virtual DWORD OnEndFrame()			{	return ccrc_continue;	}
	virtual DWORD OnBeginVisualFrame()	{	return ccrc_continue;	}
	virtual DWORD OnUpdateVisual()		{	return ccrc_continue;	}
	virtual DWORD OnEndVisualFrame()	{	return ccrc_continue;	}
	virtual DWORD OnBeginLogicalFrame()	{	return ccrc_continue;	}
	virtual DWORD OnUpdateLogical()		{	return ccrc_continue;	}
	virtual DWORD OnEndLogicalFrame()	{	return ccrc_continue;	}
};

class MANIA_ENGINE_API core
{
public:
	core( void );
	~core( void );

	log*	get_log() { return log::get_instance(); }
	timer*	get_timer() { return &m_timer; }
	static core* get_instance();
	static  void  DestroyInstance();

	//----------
	DWORD	init( HINSTANCE hInst, HWND hWnd );
	DWORD	uninit();
	void	BeginFrame();
	void	update();
	void	EndFrame();
	void	BeginVisualFrame();
	void	UpdateVisual();
	void	EndVisualFrame();
	void	BeginLogicalFrame();
	void	UpdateLogical();
	void	EndLogicalFrame();

	//----------
	rendering_thread*	get_rendering_thread() { return m_rendering_thread; }
	inputdevice*		get_inputdevice()
	{
		if ( m_inputdevice_plugin )
			return m_inputdevice_plugin->get_inputdevice();
		else
			return NULL;
	}
	BOOL	renderingthread_disabled() { return m_renderingthread_disabled;}
	void	disable_renderingthread( BOOL b ) { m_renderingthread_disabled = FALSE; }
	void	parse_cmdline( const TCHAR* cmdline );
	BOOL	get_cmdline_param( const string& paramname, string& value );
	class plugin* get_plugin( eplugintype type );

	//script-----------------
	python_vm	m_scriptvm;
	BOOL		execute_script( const string& script );
	sptr<class scene_renderer> get_scene_renderer(){ return m_scene_renderer; }
	void set_scene_renderer( sptr<class scene_renderer> sr ){ m_scene_renderer = sr; }
private:
	BOOL	BeginRenderingThread();
	BOOL	EndRenderingThread();
	BOOL	BeginSoundThread();
	BOOL	BeginLogicalThread();

	//! plugin functions
	class plugin*	load_plugin( eplugintype type );
	DWORD	initialize_plugins();


	std::vector<core_component*>	m_Components;
	rendering_thread*				m_rendering_thread;

	HWND							m_hWnd;
	HINSTANCE						m_hInstance;
	BOOL							m_renderingthread_disabled;
	timer							m_timer;
	
	sptr<class scene_renderer>		m_scene_renderer;
	renderdevice_plugin*			m_renderdevice_plugin;
	inputdevice_plugin*				m_inputdevice_plugin;

	std::vector< std::pair<string, string> > m_cmdline_params;
public:
	event main_thread_lock;
	event render_thread_lock;
};
MANIA_ENGINE_API core* get_core();
}
