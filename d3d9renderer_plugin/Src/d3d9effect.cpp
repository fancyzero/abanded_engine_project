#include "StdAfx.h"

#include "d3d9renderdevice.h"
#include "d3d9effect.h"
#include "d3d9texture.h"

namespace mania
{


MANIA_IMPLEMENT_CLASS( d3d9effect_res_base );
DWORD d3d9effect_res::create( const string& url )
{
	HRESULT hr;
	LPD3DXBUFFER buff_err;
	d3d9renderdevice* d3d9rd = static_cast<d3d9renderdevice*>( m_host->get_renderer().GetRealPointer() );
	hr = D3DXCreateEffectFromFileEx( d3d9rd->get_device(), url.c_str(), NULL, NULL, NULL,  D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, 0, &m_object, &buff_err );
	if ( FAILED( hr ) )
		get_core()->get_log()->write( 100, 0xff0000, _T( "Create d3d9effect failed, url: %s result: 0x%x \n" ), url.c_str(), hr );
	if ( buff_err )
	{
		::OutputDebugStringA(( char* )buff_err->GetBufferPointer() );
		buff_err->Release();
	}
	if ( FAILED( hr ) )
		return grc_failed;
	else
		return grc_succeeded;
}

DWORD d3d9effect_res::destroy()
{
	safe_Release( m_object );
	return grc_succeeded;
}

d3d9effect::d3d9effect( d3d9renderer_ptr renderdevice )
		: effect(( renderdevice_ptr )renderdevice )
{
}

d3d9effect::~d3d9effect( void )
{
	destroy();
}

DWORD d3d9effect::create( const string& url )
{
	m_url = url;
	m_effectres = new d3d9effect_res( this );//construct_object<d3d9effect_res>( null, null );
	return m_effectres->create( m_url );
}

DWORD d3d9effect::destroy()
{
	ENQUEUE_RENDER_TASK_BEGIN_1PARAM( task_destroy_effectres, sptr<d3d9effect_res>, res )
	return res->destroy();
	ENQUEUE_RENDER_TASK_END_1PARAM( task_destroy_effectres, m_effectres )
	return grc_succeeded;
}

DWORD d3d9effect::begin( UINT& passes, DWORD flags )
{
	if ( m_effectres == NULL )
		return grc_failed;
	HRESULT hr = m_effectres->m_object->Begin( &passes, flags );
	if ( FAILED( hr ) )
		return grc_failed;
	else
		return grc_succeeded;
}

DWORD d3d9effect::end()
{
	if ( m_effectres == NULL )
		return grc_failed;
	HRESULT hr = m_effectres->m_object->End();
	if ( FAILED( hr ) )
		return grc_failed;
	else
		return grc_succeeded;
}

DWORD d3d9effect::begin_pass( UINT pass )
{
	if ( m_effectres == NULL )
		return grc_failed;
	HRESULT hr = m_effectres->m_object->BeginPass( pass );
	if ( FAILED( hr ) )
		return grc_failed;
	else
		return grc_succeeded;
}

DWORD d3d9effect::end_pass()
{
	if ( m_effectres == NULL )
		return grc_failed;
	HRESULT hr = m_effectres->m_object->EndPass();
	if ( FAILED( hr ) )
		return grc_failed;
	else
		return grc_succeeded;
}

DWORD d3d9effect::on_lost_device()
{
	if ( m_effectres && m_effectres->m_object )
	{
		HRESULT hr = m_effectres->m_object->OnLostDevice();
		if ( SUCCEEDED( hr ) )
			return grc_succeeded;
		else
			return grc_failed;
	}
	else
		return grc_succeeded;
}

DWORD d3d9effect::on_reset_device()
{
	if ( m_effectres && m_effectres->m_object )
	{
		HRESULT hr = m_effectres->m_object->OnResetDevice();
		if ( SUCCEEDED( hr ) )
			return grc_succeeded;
		else
			return grc_failed;
	}
	else
		return grc_succeeded;
}

DWORD d3d9effect::set_param( const char* name, const math::matrix44& mat )
{
	if ( m_effectres && m_effectres->m_object )
	{
		D3DXHANDLE handle;
		handle = m_effectres->m_object->GetParameterByName( NULL, name );
		HRESULT hr = m_effectres->m_object->SetMatrix( handle, (D3DXMATRIX*)&mat );
		if ( SUCCEEDED( hr ) )
			return grc_succeeded;
		else
			return grc_failed;
	}
	else
		return grc_failed;
}

DWORD d3d9effect::set_param( const char* name, const math::matrix44 mats[], UINT count )
{
	if ( m_effectres && m_effectres->m_object )
	{
		D3DXHANDLE handle;
		handle = m_effectres->m_object->GetParameterByName( NULL, name );
		HRESULT hr = m_effectres->m_object->SetMatrixArray( handle, (D3DXMATRIX*)mats, count );
		if ( SUCCEEDED( hr ) )
			return grc_succeeded;
		else
			return grc_failed;
	}
	else
		return grc_failed;
}

DWORD d3d9effect::set_param( const char* name, float value )
{
	if ( m_effectres && m_effectres->m_object )
	{
		D3DXHANDLE handle;
		handle = m_effectres->m_object->GetParameterByName( NULL, name );
		HRESULT hr = m_effectres->m_object->SetFloat( handle, value );
		if ( SUCCEEDED( hr ) )
			return grc_succeeded;
		else
			return grc_failed;
	}
	else
		return grc_failed;
}
DWORD d3d9effect::set_param( const char* name, const float value[], UINT count )
{
	if ( m_effectres && m_effectres->m_object )
	{
		D3DXHANDLE handle;
		handle = m_effectres->m_object->GetParameterByName( NULL, name );
		HRESULT hr = m_effectres->m_object->SetFloatArray( handle, value, count );
		if ( SUCCEEDED( hr ) )
			return grc_succeeded;
		else
			return grc_failed;
	}
	else
		return grc_failed;
}
DWORD d3d9effect::set_param( const char* name, int value )
{
	if ( m_effectres && m_effectres->m_object )
	{
		D3DXHANDLE handle;
		handle = m_effectres->m_object->GetParameterByName( NULL, name );
		HRESULT hr = m_effectres->m_object->SetInt( handle, value );
		if ( SUCCEEDED( hr ) )
			return grc_succeeded;
		else
			return grc_failed;
	}
	else
		return grc_failed;
}
DWORD d3d9effect::set_param( const char* name, const int value[], UINT count )
{
	if ( m_effectres && m_effectres->m_object )
	{
		D3DXHANDLE handle;
		handle = m_effectres->m_object->GetParameterByName( NULL, name );
		HRESULT hr = m_effectres->m_object->SetIntArray( handle, value, count );
		if ( SUCCEEDED( hr ) )
			return grc_succeeded;
		else
			return grc_failed;
	}
	else
		return grc_failed;
}
//DWORD d3d9effect::set_param( char* name, BOOL value )
//{
//	if ( m_effectres && m_effectres->m_object )
//	{
//		D3DXHANDLE handle;
//		handle = m_effectres->m_object->GetParameterByName( NULL, name() );
//		HRESULT hr = m_effectres->m_object->SetBool( handle, value );
//		if ( SUCCEEDED(hr) )
//			return grc_succeeded;
//		else
//			return grc_failed;
//	}
//	else
//		return grc_failed;
//}
//DWORD d3d9effect::set_param( char* name, BOOL value[], UINT count )
//{
//	if ( m_effectres && m_effectres->m_object )
//	{
//		D3DXHANDLE handle;
//		handle = m_effectres->m_object->GetParameterByName( NULL, name() );
//		HRESULT hr = m_effectres->m_object->SetBoolArray( handle, value, count );
//		if ( SUCCEEDED(hr) )
//			return grc_succeeded;
//		else
//			return grc_failed;
//	}
//	else
//		return grc_failed;
//}
DWORD d3d9effect::set_param( const char* name, sptr<texture> tex )
{
	if ( m_effectres && m_effectres->m_object )
	{
		D3DXHANDLE handle;
		IDirect3DTexture9* tex9 = NULL;
		sptr<d3d9texture> tex9ptr = tex;
		if ( tex9ptr && tex9ptr->m_texres )
		{
			tex9 = tex9ptr->m_texres->get_object();
		}
		handle = m_effectres->m_object->GetParameterByName( NULL, name );
		HRESULT hr = m_effectres->m_object->SetTexture( handle, tex9 );
		if ( SUCCEEDED( hr ) )
			return grc_succeeded;
		else
			return grc_failed;
	}
	else
		return grc_failed;
}

}