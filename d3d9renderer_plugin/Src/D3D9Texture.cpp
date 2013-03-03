#include "stdafx.h"
#include "d3d9renderdevice.h"
#include "d3d9texture.h"

namespace mania
{
d3d9texture::d3d9texture( d3d9renderer_ptr renderdevice )
		: texture(( renderdevice_ptr )renderdevice )
{}

d3d9texture::~d3d9texture( void )
{
	destroy();
}

DWORD d3d9texture::create( int w, int h, int mip_count, int usage, int format, int pool )
{
	m_texres = new d3d9texture_res( this );
	return m_texres->create( w, h, mip_count, usage, format, pool );
}

DWORD d3d9texture::create( const string& url, int mip_count, int usage, int format, int pool )
{
	HRESULT hr;
	m_url = url;
	m_texres = new d3d9texture_res( this );
	hr = D3DXCreateTextureFromFileEx((( sptr<d3d9renderdevice> )m_renderdevice )->get_device(), url.c_str(), D3DX_DEFAULT,
	                                 D3DX_DEFAULT, mip_count, usage, ( D3DFORMAT )format, ( D3DPOOL )pool,
	                                 D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_texres->m_object );
	if ( SUCCEEDED( hr ) )
		return grc_succeeded;
	else
		return grc_failed;
}

DWORD d3d9texture::create( const void* dataaddr, DWORD datasize, int mip_count, int usage, int format, int pool )
{
	//HRESULT hr;
	//m_texres = new D3D9TextureRes();
	//url = filename
	//hr = D3DXCreateTextureFromFileInMemoryEx( m_renderdevice->get_device(), dataaddr, datasize, D3DX_DEFAULT,
	//	D3DX_DEFAULT, mip_count, usage, (D3DFORMAT)format, (D3DPOOL)pool,
	//	D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_texres->m_d3d9texture );
	//if ( SUCCEEDED(hr) )
	//	return grc_succeeded;
	//else
	return grc_failed;
}

DWORD d3d9texture::get_surface( int level, IDirect3DSurface9 ** surface9 )
{
	*surface9 = NULL;
	if ( m_texres == NULL || m_texres->get_object() == NULL )
		return grc_failed;
	HRESULT hr;
	hr = m_texres->get_object()->GetSurfaceLevel( level, surface9 );
	if ( SUCCEEDED( hr ) )
		return grc_succeeded;
	else
		return grc_failed;
}

DWORD d3d9texture::destroy()
{
	return m_texres->destroy();
}

gresult	d3d9texture::on_lost_device()
{
	if ( m_texres == NULL || m_texres->get_object() == NULL )
		return grc_succeeded;
	//TODO: take care unmanaged texture;
	return grc_succeeded;
}

gresult	d3d9texture::on_reset_device()
{
	if ( m_texres == NULL || m_texres->get_object() == NULL )
		return grc_succeeded;
	//TODO: take care unmanaged texture;
	return grc_succeeded;
}

////////////////////////////////
//texture res
////////////////////////////////


MANIA_IMPLEMENT_CLASS( d3d9texture_res_base );

DWORD d3d9texture_res::create( int w, int h, int mip_count, int usage, int format, int pool )
{
	HRESULT hr;
	d3d9renderdevice* d3d9rd = static_cast<d3d9renderdevice*>( m_host->get_renderer().GetRealPointer() );
	hr = D3DXCreateTexture( d3d9rd->get_device(), w, h, mip_count, usage, ( D3DFORMAT )format, ( D3DPOOL )pool, &m_object );
	if ( SUCCEEDED( hr ) )
		return grc_succeeded;
	else
		return grc_failed;
}
DWORD d3d9texture_res::create( const string& url, int mip_count, int usage, int format, int pool )
{
	return grc_succeeded;
}
DWORD d3d9texture_res::create( const void* dataaddr, DWORD datasize, int mip_count, int usage, int format, int pool )
{
	return grc_succeeded;
}
DWORD d3d9texture_res::destroy()
{
	safe_Release( m_object );
	return grc_succeeded;
}
}