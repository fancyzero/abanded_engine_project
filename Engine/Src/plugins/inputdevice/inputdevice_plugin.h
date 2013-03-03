#pragma once

namespace mania
{
class inputdevice_plugin : public plugin
{
public:
	virtual void get_desc( plugin_desc& desc ) = 0;
	virtual class inputdevice* get_inputdevice() = 0;
};
}
#include "inputdevice.h"