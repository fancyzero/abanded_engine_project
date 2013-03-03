#pragma once

class mod : public mania::object
{
public:
	mod( HINSTANCE ainst, HWND amain_window );
	virtual ~mod();

	virtual mania::gresult		init()		= 0;
	virtual mania::gresult		uninit()	= 0;
	virtual mania::gresult		frame()		= 0;
protected:
	HINSTANCE		m_instance;
	HWND			m_main_window;
	mania::console*	m_console;
};
