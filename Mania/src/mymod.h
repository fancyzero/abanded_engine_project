#pragma once

using namespace mania;

enum mode_stage
{
	ms_title,
	ms_gameplay,
};

class mod_context
{
public:
	mod_context()
			: m_stage( ms_title )
	{}

	mode_stage m_stage;
};

class mymod : public mod
{
public:
	mymod( HINSTANCE ainst, HWND amain_window );
	virtual ~mymod();

	virtual gresult		init();
	virtual gresult		uninit();
	virtual gresult		frame();

	gresult		title_frame();
	gresult		gameplay_frame();
protected:
	mod_context					m_context;
	sptr<class scene>			m_scene;
	sptr<class scene_renderer>	m_renderer;
	sptr<class camera>			m_camera;
};
