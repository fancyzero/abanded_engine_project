#include "stdafx.h"
#include "d3d9renderdevice.h"
#include "D3D9IndexBuffer.h"

namespace mania
{
MANIA_IMPLEMENT_CLASS( d3d9indexbuffer_res_base );
DWORD d3d9indexbuffer_res::destroy()
{
	safe_Release( m_object );
	return grc_succeeded;
}

d3d9indexbuffer::~d3d9indexbuffer()
{
	destroy();
}
DWORD d3d9indexbuffer::create( const indexbuffer::desc_t& desc )
{
	m_desc = desc;
	m_d3d9indexbuffer_res = new d3d9indexbuffer_res( this );
	m_d3d9indexbuffer_res->create( desc );
	return grc_succeeded;
}
DWORD d3d9indexbuffer::lock( UINT lockstart, UINT locksize, void** lockeddata, DWORD flags )
{
	if ( m_d3d9indexbuffer_res != NULL )
		if ( m_d3d9indexbuffer_res->get_object() != NULL )
		{
			HRESULT hr;
			hr = m_d3d9indexbuffer_res->get_object()->Lock( lockstart, locksize, lockeddata, flags );
			if ( FAILED( hr ) )
				return grc_failed;
			else
				return grc_succeeded;
		}
	return grc_failed;
}

DWORD d3d9indexbuffer::unlock()
{
	if ( m_d3d9indexbuffer_res != NULL )
		if ( m_d3d9indexbuffer_res->get_object() != NULL )
		{
			HRESULT hr;
			hr = m_d3d9indexbuffer_res->get_object()->Unlock();
			if ( FAILED( hr ) )
				return grc_failed;
			else
				return grc_succeeded;
		}
	return grc_failed;
}

DWORD d3d9indexbuffer::destroy()
{
	ENQUEUE_RENDER_TASK_BEGIN_1PARAM( task_destroy_indexbuffer, sptr<d3d9indexbuffer_res>, res )
			return res->destroy();
	ENQUEUE_RENDER_TASK_END_1PARAM( task_destroy_indexbuffer, m_d3d9indexbuffer_res )	
	return grc_succeeded;
}

gresult	d3d9indexbuffer::on_lost_device()
{
	if ( m_d3d9indexbuffer_res == NULL || m_d3d9indexbuffer_res->get_object() == NULL )
		return grc_succeeded;
	//TODO: take care unmanaged texture;
	return grc_succeeded;
}

gresult	d3d9indexbuffer::on_reset_device()
{
	if ( m_d3d9indexbuffer_res == NULL || m_d3d9indexbuffer_res->get_object() == NULL )
		return grc_succeeded;
	//TODO: take care unmanaged texture;
	return grc_succeeded;
}
///////////////////////////////
//////////////////////////////
DWORD d3d9indexbuffer_res::create( const indexbuffer::desc_t& desc )
{
	d3d9renderdevice* d3d9rd = ( d3d9renderdevice* )m_host->get_renderer().GetRealPointer();
	if ( d3d9rd == NULL )
		return grc_failed;
	HRESULT hr;
	hr = d3d9rd->get_device()->CreateIndexBuffer( desc.size,
	        d3d9renderdevice::convert2d3d( desc.usage ),
	        d3d9renderdevice::convert2d3d( desc.format ),
	        d3d9renderdevice::convert2d3d( desc.pool ),
	        &m_object, NULL );
	if ( FAILED( hr ) )
		return grc_failed;
	else
		return grc_succeeded;
}
}