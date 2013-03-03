#include "stdafx.h"
#include "inputdevice_plugin.h"
#include "inputdevice.h"

namespace mania
{
void inputdevice_plugin_impl::get_desc( plugin_desc& desc )
{
	desc.name = _T( "windows default inputdevice plugin" );
	desc.type = plugintype_inputdevice;
}

inputdevice* inputdevice_plugin_impl::get_inputdevice()
{
	if ( m_inputdevice )
		return m_inputdevice;
	else
		return m_inputdevice = new inputdevice_win();
}

gresult inputdevice_win::update_state()
{
	::GetKeyboardState(( BYTE* )m_state.key_states );
	return grc_succeeded;
}

gresult inputdevice_win::get_input_state( input_state* state )
{
	memcpy( state, &m_state, 256 );
	return grc_succeeded;
}
}