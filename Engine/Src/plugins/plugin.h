#pragma once

namespace mania
{
enum eplugintype
{
	plugintype_renderer,
	plugintype_inputdevice,
};

struct plugin_desc
{
	string name;
	eplugintype type;
};

class plugin
{
public:
	///// Initialize the dll
	///** Call after load the dll*/
	//virtual void initialize() = 0;
	///// Uninitialize the dll
	///** Call before unload the dll*/
	//virtual void uninitialize() = 0;
	///// Get description of the plugin
	virtual void get_desc( plugin_desc& desc ) = 0;
};
}