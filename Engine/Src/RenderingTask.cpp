#include "stdafx.h"
#include "renderingtask.h"


namespace mania
{
render_task::render_task( rendering_thread* rendering_thread )
		: m_rendering_thread( rendering_thread ), m_type( rendertask_normal )
{
}

render_task::~render_task( void )
{
}

//useful render tasks
DWORD rendertask_create_vertexbuffer::execute()
{
	gresult gr = m_rendering_thread->get_renderdevice()->create_vertexbuffer( m_vb_desc, *m_vb_created );
	if ( failed( gr ) )
		return gr;
	if ( m_fill_buffer && m_fill_buffer->get_size() )
	{
		void* lockeddata;
		( *m_vb_created )->lock( 0, 0, &lockeddata, 0 );
		memcpy( lockeddata, m_fill_buffer->get_data(), m_fill_buffer->get_size() );
		( *m_vb_created )->unlock();
	}
	return grc_succeeded;
}

DWORD rendertask_create_indexbuffer::execute()
{
	gresult gr = m_rendering_thread->get_renderdevice()->create_indexbuffer( m_ib_desc, *m_ib_created );
	if ( failed( gr ) )
		return gr;
	if ( m_fill_buffer && m_fill_buffer->get_size() )
	{
		void* lockeddata;
		( *m_ib_created )->lock( 0, 0, &lockeddata, 0 );
		memcpy( lockeddata, m_fill_buffer->get_data(), m_fill_buffer->get_size() );
		( *m_ib_created )->unlock();
	}
	return grc_succeeded;
}

DWORD rendertask_create_vertexdeclearation::execute()
{
	gresult gr = m_rendering_thread->get_renderdevice()->create_vertexddecl( m_ve_def, *m_vd_created );
	if ( failed( gr ) )
		return gr;
	return grc_succeeded;
}
}