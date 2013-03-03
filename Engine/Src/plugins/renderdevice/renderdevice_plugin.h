#include "renderer_type.h"
#include "renderresource.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "renderdevice.h"
#include "effect.h"
#include "texture.h"
#include "vertex_declaration.h"
#include "font.h"
#include "effect_pool.h"

#pragma once


namespace mania
{
class renderdevice_plugin : public plugin
{
public:
	virtual void get_desc( plugin_desc& desc ) = 0;
	virtual sptr<class renderdevice> create_renderer() = 0;
};
}