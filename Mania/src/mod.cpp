#include "stdafx.h"
#include "mod.h"

using namespace mania;
mod::mod( HINSTANCE ainst, HWND amain_window )
		: m_instance( ainst ), m_main_window( amain_window )
{
	m_console = mania::console::create();
	m_console->init();
}

mod::~mod()
{
	safe_delete( m_console );
}