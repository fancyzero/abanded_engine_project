#include "stdafx.h"
#include "d3d9renderdevice_plugin.h"
#include "d3d9renderdevice.h"

namespace mania
{
D3D9RD_DLL_API mania::plugin* create_plugin()
{
	get_core()->get_log()->write( 1, 0x00ff00, _T( "mania d3d9renderdevice plugin created.\n" ) );
	return new d3d9renderdevice_plugin();
}

void d3d9renderdevice_plugin::get_desc( plugin_desc& desc )
{
	desc.name = _T( "mania d3d9renderdevice plugin" );
	desc.type = plugintype_renderer;
}

sptr<renderdevice> d3d9renderdevice_plugin::create_renderer()
{
	return new d3d9renderdevice();
}
}