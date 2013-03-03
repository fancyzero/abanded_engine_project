#include "stdafx.h"
#include "d3d9renderdevice.h"
#include "D3D9VertexBuffer.h"



namespace mania
{
	d3d9vertexbuffer::~d3d9vertexbuffer( void )
	{
		destroy();
	}
	DWORD d3d9vertexbuffer::create( const vertexbuffer::desc_t& desc )
	{
		m_desc = desc;
		m_d3d9vertexbuffer_res = new d3d9vertexbuffer_res( this );
		m_d3d9vertexbuffer_res->create( desc );
		return grc_succeeded;
	}

	DWORD d3d9vertexbuffer::destroy()
	{
		ENQUEUE_RENDER_TASK_BEGIN_1PARAM( task_destroy_vertexbuffer, sptr<d3d9vertexbuffer_res>, res )
			return res->destroy();
		ENQUEUE_RENDER_TASK_END_1PARAM( task_destroy_vertexbuffer, m_d3d9vertexbuffer_res )	
		return grc_succeeded;
	}

	DWORD d3d9vertexbuffer::lock( UINT lockstart, UINT locksize, void** lockeddata, DWORD flags )
	{
		if ( m_d3d9vertexbuffer_res != NULL )
			if ( m_d3d9vertexbuffer_res->get_object() != NULL )
			{
				HRESULT hr;
				hr = m_d3d9vertexbuffer_res->get_object()->Lock( lockstart, locksize, lockeddata, flags );
				if ( FAILED( hr ) )
					return grc_failed;
				else
					return grc_succeeded;
			}
			return grc_failed;
	}

	DWORD d3d9vertexbuffer::unlock()
	{
		if ( m_d3d9vertexbuffer_res != NULL )
			if ( m_d3d9vertexbuffer_res->get_object() != NULL )
			{
				HRESULT hr;
				hr = m_d3d9vertexbuffer_res->get_object()->Unlock();
				if ( FAILED( hr ) )
					return grc_failed;
				else
					return grc_succeeded;
			}
			return grc_failed;
	}

	gresult	d3d9vertexbuffer::on_lost_device()
	{
		if ( m_d3d9vertexbuffer_res == NULL || m_d3d9vertexbuffer_res->get_object() == NULL )
			return grc_succeeded;
		//TODO: take care unmanaged texture;
		return grc_succeeded;
	}

	gresult	d3d9vertexbuffer::on_reset_device()
	{
		if ( m_d3d9vertexbuffer_res == NULL || m_d3d9vertexbuffer_res->get_object() == NULL )
			return grc_succeeded;
		//TODO: take care unmanaged texture;
		return grc_succeeded;
	}

	//vertexbuffer_res

	MANIA_IMPLEMENT_CLASS( d3d9vertexbuffer_res_base );

	DWORD d3d9vertexbuffer_res::create( const vertexbuffer::desc_t& desc )
	{
		d3d9renderdevice* d3d9rd = ( d3d9renderdevice* )m_host->get_renderer().GetRealPointer();
		if ( d3d9rd == NULL )
			return grc_failed;
		HRESULT hr;
		hr = d3d9rd->get_device()->CreateVertexBuffer( desc.size,
			d3d9renderdevice::convert2d3d( desc.usage ),
			0,
			d3d9renderdevice::convert2d3d( desc.pool ),
			&m_object, NULL );
		if ( FAILED( hr ) )
			return grc_failed;
		else
			return grc_succeeded;
	}

	DWORD d3d9vertexbuffer_res::destroy()
	{
		safe_Release( m_object );
		return grc_succeeded;
	}
}