#include "StdAfx.h"
#include "widget.h"

classinfo widget::static_classinfo;

widget::widget( void )
		: m_x( 0.0f ), m_y( 0.0f ), m_w( 0.0f ), m_h( 0.0f )
{
	MANIA_CLASS_CONSTRUCTOR_INCLUDE( widget )
}

widget::~widget( void )
{}

DWORD widget::create( string effect_url, string texture_url )
{
	struct widget_create_param
	{
		string effect_url;
		string texture_url;
	};
	widget_create_param cp;
	cp.effect_url = effect_url;
	cp.texture_url = texture_url;
	ENQUEUE_RENDER_TASK_BEGIN_2PARAM( createwidget, sptr<widget>, wdg2create, widget_create_param, createparam )
	mania::get_core()->get_rendering_thread()->get_renderdevice()->create_effect_from_url( _T( "simple" ), createparam.effect_url, wdg2create->m_effect );
	mania::get_core()->get_rendering_thread()->get_renderdevice()->create_texture_from_url( _T( "simpletex" ), createparam.texture_url, 0, usage_none, fmt_argb, pool_managed, wdg2create->m_texture );
	return grc_succeeded;
	ENQUEUE_RENDER_TASK_END_2PARAM( createwidget, this, cp );
	return grc_succeeded;
}

DWORD widget::destroy()
{
	ENQUEUE_RENDER_TASK_BEGIN_1PARAM( destroywidget, sptr<widget>, wdg2destroy )
	wdg2destroy->m_effect = NULL;
	wdg2destroy->m_texture = NULL;
	return grc_succeeded;
	ENQUEUE_RENDER_TASK_END_1PARAM( destroywidget, this );
	return grc_succeeded;
}