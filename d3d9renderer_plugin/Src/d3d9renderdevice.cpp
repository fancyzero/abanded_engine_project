#include "stdafx.h"
#include "d3d9renderdevice.h"
#include "d3d9texture.h"
#include "d3d9effect.h"
#include "D3D9VertexBuffer.h"
#include "D3D9indexBuffer.h"
#include "d3d9vertex_declaration.h"

namespace mania
{
d3d9renderdevice::d3d9renderdevice( void )
		: m_dwBackgroundColor( 0 ),
		m_pHostThread( NULL ),
		m_pDevice( NULL )
{
	ZeroMemory( m_backup_rendertarget, sizeof m_backup_rendertarget );
}

d3d9renderdevice::~d3d9renderdevice( void )
{
	//Destroy();
}

DWORD d3d9renderdevice::SetupD3DPP()
{
	ZeroMemory( &m_D3DPP, sizeof m_D3DPP );
	m_D3DPP.AutoDepthStencilFormat = D3DFMT_D16;
	m_D3DPP.BackBufferFormat = D3DFMT_A8R8G8B8;
	m_D3DPP.BackBufferCount = m_create_param.backbuffer_count;
	m_D3DPP.BackBufferWidth = m_create_param.w;
	m_D3DPP.BackBufferHeight = m_create_param.h;
	m_D3DPP.EnableAutoDepthStencil = TRUE;
	m_D3DPP.SwapEffect = D3DSWAPEFFECT_FLIP;
	m_D3DPP.Windowed = m_create_param.windowed;
	m_D3DPP.hDeviceWindow = m_create_param.host_window;
	return grc_succeeded;
}

DWORD d3d9renderdevice::Create( renderer_create_param param )
{
	m_create_param = param;
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	_ASSERT( m_pD3D );
	return reset( m_create_param );
}

DWORD d3d9renderdevice::Destroy()
{
	unsigned long ref = 0;
	if ( m_pDevice != NULL )
		ref = m_pDevice->Release();
	m_pDevice = NULL;
	if ( m_pD3D != NULL )
		ref = m_pD3D->Release();
	m_pD3D = NULL;
	return grc_succeeded;
}

DWORD d3d9renderdevice::begin_frame()
{
	HRESULT hr = m_pDevice->BeginScene();
	if ( FAILED( hr ) )
		return grc_failed;
	hr = m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_dwBackgroundColor, 1.0f, 0 );
	if ( FAILED( hr ) )
		return grc_failed;
	return grc_succeeded;
}

DWORD d3d9renderdevice::end_frame()
{
	HRESULT hr = m_pDevice->EndScene();
	if ( FAILED( hr ) )
		return grc_failed;
	hr = m_pDevice->Present( NULL, NULL, NULL, NULL );
	if ( FAILED( hr ) )
		return grc_failed;
	return grc_succeeded;
}

//-----------------
void d3d9renderdevice::SetBackgroundColor( DWORD dwColor )
{
	m_dwBackgroundColor = dwColor;
}

//texture
DWORD d3d9renderdevice::create_texture( const string& name, int w, int h, int mip_count, int usage, int format, int pool, sptr<texture>& texLoaded )
{
	_ASSERT( m_pDevice );
	d3d9texture*	pNewTex;
	pNewTex = new d3d9texture( this );
	pNewTex->create( w, h, 0, 0, 0, 0 );
	m_texture_storage.add_item( name, pNewTex );
	texLoaded = pNewTex;
	return grc_succeeded;
}

DWORD d3d9renderdevice::create_texture_from_url( const string& name, const string& url,
        int mip_count, int usage, int format, int pool, sptr<texture>& texLoaded )
{
	_ASSERT( m_pDevice );
	d3d9texture*	newtex;
	newtex = new d3d9texture( this );
	m_texture_storage.add_item( name, newtex );
	texLoaded = newtex;
	return newtex->create( url, mip_count, usage, format, pool );
}

DWORD d3d9renderdevice::create_texture_from_mem( const string& name, const void* dataaddr,
        DWORD datasize, int mip_count, int usage, int format, int pool, sptr<texture>& texLoaded )
{
	_ASSERT( m_pDevice );
	d3d9texture*	newtex;
	newtex = new d3d9texture( this );
	return newtex->create( dataaddr, datasize, mip_count, usage, format, pool );
}

DWORD d3d9renderdevice::destroy_texture( texture* spTex )
{
	return grc_succeeded;
}

//effect

DWORD d3d9renderdevice::create_effect_from_url( const string& name, const string& url, sptr<effect>& effcreated )
{
	DWORD ret;
	d3d9effect* neweff;
	effcreated = neweff = new d3d9effect( this );
	m_effect_storage.add_item( name, neweff );
	neweff->set_name( name );
	ret = neweff->create( url );
	if ( ret != grc_succeeded )
		effcreated = NULL;
	return ret;
}
DWORD d3d9renderdevice::destroy_effect( effect* eff )
{
	if ( !m_effect_storage.remove_item( eff->get_name() ) )
		return grc_failed;
	return eff->destroy();
}
// rendering

DWORD d3d9renderdevice::draw_indexedprimitive( eprimitivetype type, int base_index, UINT min_index, UINT num_vertices, int start_index,  UINT count )
{
	HRESULT hr;
	hr = m_pDevice->DrawIndexedPrimitive(
	         convert2d3d( type ),
	         base_index,
	         min_index,
	         num_vertices,
	         start_index,
	         count );
	if ( FAILED( hr ) )
		return grc_failed;
	else
		return grc_succeeded;
}

DWORD d3d9renderdevice::draw_indexedprimitive_mem( void* vertex_data, UINT vertex_stride, void* index_data, eformat index_format, eprimitivetype type, UINT min_index, UINT num_vertices,  UINT count )
{
	HRESULT hr;
	m_pDevice->SetStreamSourceFreq( 0, 1 );
	hr = m_pDevice->DrawIndexedPrimitiveUP(
	         convert2d3d( type ),
	         min_index,
	         num_vertices,
	         count,
	         index_data,
	         convert2d3d( index_format ),
	         vertex_data,
	         vertex_stride );
	if ( FAILED( hr ) )
		return grc_failed;
	else
		return grc_succeeded;
}

//vb ib
DWORD d3d9renderdevice::create_vertexbuffer( const vertexbuffer::desc_t&  desc, sptr<vertexbuffer>& vb_created )
{
	vb_created = new d3d9vertexbuffer( this );
	vb_created->create( desc );
	return grc_succeeded;
}

DWORD d3d9renderdevice::create_indexbuffer( const indexbuffer::desc_t&  desc, sptr<indexbuffer>& ib_created )
{
	ib_created = new d3d9indexbuffer( this );
	ib_created->create( desc );
	return grc_succeeded;
}

//vertx decl
DWORD d3d9renderdevice::create_vertexddecl( const std::vector<vertexelem_def>& elemdefs, sptr<vertex_declaration>& vd )
{
	D3DVERTEXELEMENT9* d3d9elems = new D3DVERTEXELEMENT9[elemdefs.size()+1];
	UINT i = 0;
	for ( ; i < elemdefs.size(); i++ )
	{
		d3d9elems[i].Method = convert2d3d( elemdefs[i].process_method );
		d3d9elems[i].Offset = ( short )elemdefs[i].offset;
		d3d9elems[i].Stream = ( short )elemdefs[i].stream;
		d3d9elems[i].Type = convert2d3d( elemdefs[i].type );
		d3d9elems[i].Usage = convert2d3d( elemdefs[i].usage );
		d3d9elems[i].UsageIndex = ( BYTE )elemdefs[i].usage_index;
	}
	d3d9elems[i].Stream = 0xff;
	d3d9elems[i].Offset = 0;
	d3d9elems[i].Type = D3DDECLTYPE_UNUSED;
	d3d9elems[i].Method = 0;
	d3d9elems[i].Usage = 0;
	d3d9elems[i].UsageIndex = 0;
	d3d9vertex_declaration* newdecl = new d3d9vertex_declaration();
	HRESULT hr = m_pDevice->CreateVertexDeclaration( d3d9elems, &newdecl->m_d3dvertexdecl9 );
	delete[] d3d9elems;
	vd = newdecl;
	if ( FAILED( hr ) )
		return grc_failed;
	else
		return grc_succeeded;
}

DWORD d3d9renderdevice::set_vertexdecl( sptr<vertex_declaration> vd )
{
	sptr<d3d9vertex_declaration> d3d9vd = ( d3d9vertex_declaration* )( vd.GetRealPointer() );
	HRESULT hr = m_pDevice->SetVertexDeclaration( d3d9vd->m_d3dvertexdecl9 );
	if ( FAILED( hr ) )
		return grc_failed;
	else
		return grc_succeeded;
}

//STATES
DWORD d3d9renderdevice::set_texture( UINT index, sptr<texture> tex )
{
	d3d9texture* d3d9tex = ( d3d9texture* )tex.GetRealPointer();
	if ( d3d9tex == NULL || d3d9tex->m_texres == NULL )
	{
		m_pDevice->SetTexture( index, NULL );
		return grc_succeeded;
	}
	HRESULT hr = m_pDevice->SetTexture( index, d3d9tex->m_texres->get_object() );
	if ( SUCCEEDED( hr ) )
		return grc_succeeded;
	else
		return grc_failed;
}

DWORD d3d9renderdevice::set_vertexbuffer( UINT index, sptr<vertexbuffer> vb )
{
	d3d9vertexbuffer* d3d9vb = ( d3d9vertexbuffer* )vb.GetRealPointer();
	if ( d3d9vb != NULL && d3d9vb->m_d3d9vertexbuffer_res != NULL )
	{
		IDirect3DVertexBuffer9* id3dvb = d3d9vb->m_d3d9vertexbuffer_res->get_object();
		m_pDevice->SetStreamSource( index, (( d3d9vertexbuffer* )vb.GetRealPointer() )->m_d3d9vertexbuffer_res->get_object(), 0, vb->get_stride() );
	}
	return grc_succeeded;
}

DWORD d3d9renderdevice::set_indexbuffer( UINT index, sptr<indexbuffer> ib )
{
	d3d9indexbuffer* d3d9ib = ( d3d9indexbuffer* )ib.GetRealPointer();
	if ( d3d9ib != NULL && d3d9ib->m_d3d9indexbuffer_res != NULL )
	{
		IDirect3DIndexBuffer9* id3dib = d3d9ib->m_d3d9indexbuffer_res->get_object();
		m_pDevice->SetIndices((( d3d9indexbuffer* )ib.GetRealPointer() )->m_d3d9indexbuffer_res->get_object() );
	}
	return grc_succeeded;
}

DWORD d3d9renderdevice::set_rendertarget( UINT index, sptr<texture> texture )
{
	_ASSERT( m_backup_rendertarget[index] == NULL );//can not set rendertarget more than once, restore first
	HRESULT hr;
	if ( texture == NULL )
		hr = m_pDevice->SetRenderTarget( index, NULL );
	else
	{
		hr = m_pDevice->GetRenderTarget( index, &m_backup_rendertarget[index] );
		if ( FAILED( hr ) )
		{
			get_core()->get_log()->write( 100, 0xff0000, _T( "Can not get rendertarget! \n" ) );
			return grc_failed;
		}
		sptr<d3d9texture> d3d9tex = texture;
		IDirect3DSurface9* surface9;
		if ( d3d9tex->get_surface( 0, &surface9 ) != grc_succeeded )
		{
			get_core()->get_log()->write( 100, 0xff0000, _T( "get surface failed! \n" ) );
			return grc_failed;
		}
		hr = m_pDevice->SetRenderTarget( index, surface9 );
	}
	if ( SUCCEEDED( hr ) )
		return grc_succeeded;
	else
		return grc_failed;
}

DWORD d3d9renderdevice::restore_rendertarget( UINT index )
{
	HRESULT hr;
	IDirect3DSurface9* surface9;
	hr = m_pDevice->GetRenderTarget( index, &surface9 );
	if ( FAILED( hr ) )
		return grc_failed;
	surface9->Release();//release for getrendertarget()
	surface9->Release();//release for get_surface()
	hr = m_pDevice->SetRenderTarget( index, m_backup_rendertarget[index] );
	if ( SUCCEEDED( hr ) )
	{
		safe_Release( m_backup_rendertarget[index] );
		return grc_succeeded;
	}
	else
	{
		get_core()->get_log()->write( 100, 0xff0000, _T( "restore_rendertarget failed! \n" ) );
		return grc_failed;
	}
}

DWORD d3d9renderdevice::reset( renderer_create_param param )
{
	m_create_param = param;
	if ( SetupD3DPP() != grc_succeeded )
		return grc_failed;
	if ( m_pDevice == NULL )
	{
		// Set default settings
		UINT AdapterToUse = D3DADAPTER_DEFAULT;
		D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;
#if !_DEBUG
		// When building a shipping version, disable PerfHUD (opt-out)
#else
		// Look for 'NVIDIA PerfHUD' adapter
		// If it is present, override default settings
		for ( UINT Adapter = 0; Adapter < m_pD3D->GetAdapterCount(); Adapter++ )
		{
			D3DADAPTER_IDENTIFIER9 Identifier;
			HRESULT Res;
			Res = m_pD3D->GetAdapterIdentifier( Adapter, 0, &Identifier );
			if ( strstr( Identifier.Description, "PerfHUD" ) != 0 )
			{
				AdapterToUse = Adapter;
				DeviceType = D3DDEVTYPE_REF;
				break;
			}
		}
#endif
		HRESULT hr = m_pD3D->CreateDevice( AdapterToUse, DeviceType, m_create_param.host_window, D3DCREATE_MIXED_VERTEXPROCESSING , &m_D3DPP, &m_pDevice );
		_ASSERT( SUCCEEDED( hr ) );
		if ( SUCCEEDED( hr ) )
		{
			return grc_succeeded;
		}
		return grc_failed;
	}
	else
	{
		for ( storage<d3d9texture*>::iterator it = m_texture_storage.begin(); it != m_texture_storage.end(); ++it )
			( *it ).second->on_lost_device();
		for ( storage<d3d9effect*>::iterator it = m_effect_storage.begin(); it != m_effect_storage.end(); ++it )
			( *it ).second->on_lost_device();
		for ( storage<d3d9vertexbuffer*>::iterator it = m_vertexbuffer_storage.begin(); it != m_vertexbuffer_storage.end(); ++it )
			( *it ).second->on_lost_device();
		for ( storage<d3d9indexbuffer*>::iterator it = m_indexbuffer_storage.begin(); it != m_indexbuffer_storage.end(); ++it )
			( *it ).second->on_lost_device();
		HRESULT hr = m_pDevice->Reset( &m_D3DPP );
		for ( storage<d3d9texture*>::iterator it = m_texture_storage.begin(); it != m_texture_storage.end(); ++it )
			( *it ).second->on_reset_device();
		for ( storage<d3d9effect*>::iterator it = m_effect_storage.begin(); it != m_effect_storage.end(); ++it )
			( *it ).second->on_reset_device();
		for ( storage<d3d9vertexbuffer*>::iterator it = m_vertexbuffer_storage.begin(); it != m_vertexbuffer_storage.end(); ++it )
			( *it ).second->on_reset_device();
		for ( storage<d3d9indexbuffer*>::iterator it = m_indexbuffer_storage.begin(); it != m_indexbuffer_storage.end(); ++it )
			( *it ).second->on_reset_device();
		if ( SUCCEEDED( hr ) )
			return grc_succeeded;
		else
			return grc_failed;
	}
}
}
