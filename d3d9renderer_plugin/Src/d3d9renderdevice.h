#include "..\..\engine\src\plugins\renderdevice\renderdevice.h"
#include <D3D9.h>
#include <D3DX9.h>
#pragma once

namespace mania
{
class texture;
class d3d9texture;
class d3d9effect;
class d3d9vertexbuffer;
class d3d9indexbuffer;
class d3d9renderdevice : public renderdevice
{
public:
	d3d9renderdevice( void );
	~d3d9renderdevice( void );

	virtual DWORD Create( renderer_create_param );
	virtual DWORD Destroy();

	//scene rendering
	virtual DWORD begin_frame();
	virtual DWORD end_frame();

	//---------------
	virtual void SetBackgroundColor( DWORD dwColor );

	//Threading
	virtual void SetHostThread( rendering_thread* pThread )
	{
		m_pHostThread = pThread;
	}

	IDirect3DDevice9* get_device()
	{
		return m_pDevice;
	}

	//convert
	static D3DPRIMITIVETYPE convert2d3d( eprimitivetype type ) { return ( D3DPRIMITIVETYPE ) type; }
	static D3DPOOL convert2d3d( epool p ) { return ( D3DPOOL ) p; }
	static D3DFORMAT convert2d3d( eformat f )
	{
		switch ( f )
		{
		case fmt_rgb:
			return D3DFMT_R8G8B8;
		case fmt_argb:
			return D3DFMT_A8R8G8B8;
		case fmt_xrgb:
			return D3DFMT_X8R8G8B8;
		case fmt_argb_f16:
			return D3DFMT_A16B16G16R16;
		case fmt_index16:
			return D3DFMT_INDEX16;
		case fmt_index32:
			return D3DFMT_INDEX32;
		default:
			return D3DFMT_UNKNOWN;
		}
	}
	static DWORD convert2d3d( eusage p ) { return p; }
	static D3DDECLTYPE convert2d3d( mania::edecltype p ) { return ( D3DDECLTYPE )p; }
	static D3DDECLMETHOD convert2d3d( mania::edeclmethod p ) { return ( D3DDECLMETHOD )p; }
	static D3DDECLUSAGE convert2d3d( mania::edeclusage p ) { return ( D3DDECLUSAGE )p; }
	//texture
	virtual DWORD create_texture( const string& name, int w, int h, int mip_count, int usage, int format, int pool, sptr<texture>& texLoaded );
	virtual DWORD create_texture_from_url( const string& name, const string& url, int mip_count,  int usage, int format , int pool, sptr<texture>& texLoaded );
	virtual DWORD create_texture_from_mem( const string& name, const void* dataaddr, DWORD datasize, int mip_count,  int usage, int format , int pool, sptr<texture>& texLoaded );
	virtual DWORD destroy_texture( texture* spTex );

	//effect
	virtual DWORD create_effect_from_url( const string& name, const string& url, sptr<effect>& effcreated );
	virtual DWORD destroy_effect( effect* spTex );
	// rendring
	virtual DWORD draw_indexedprimitive( eprimitivetype type, int base_index, UINT min_index, UINT num_vertices, int start_index,  UINT count );
	virtual DWORD draw_indexedprimitive_mem( void* vertex_data, UINT vertex_stride, void* index_data, eformat index_format, eprimitivetype type, UINT min_index, UINT num_vertices,  UINT count );
	//vb ib
	virtual DWORD create_vertexbuffer( const vertexbuffer::desc_t& desc, sptr<vertexbuffer>& vb_created );
	virtual DWORD create_indexbuffer( const indexbuffer::desc_t& desc, sptr<indexbuffer>& ib_created );
	//vertex decl
	virtual DWORD create_vertexddecl( const std::vector<vertexelem_def>& elemdefs, sptr<vertex_declaration>& vd );
	virtual DWORD set_vertexdecl( sptr<vertex_declaration> vd );

	//states
	virtual DWORD set_texture( UINT index, sptr<texture> tex );
	virtual DWORD set_vertexbuffer( UINT index, sptr<vertexbuffer> vb );
	virtual DWORD set_indexbuffer( UINT index, sptr<indexbuffer> ib );
	virtual DWORD set_rendertarget( UINT index, sptr<texture> texture );
	virtual DWORD restore_rendertarget( UINT index );
	//misc
	virtual DWORD reset( renderer_create_param param );

public:
	renderer_create_param get_create_param() { return m_create_param; }

private:
	DWORD SetupD3DPP();
	IDirect3D9*					m_pD3D;
	IDirect3DDevice9*			m_pDevice;
	D3DPRESENT_PARAMETERS		m_D3DPP;
	rendering_thread*			m_pHostThread;
	storage<d3d9texture*>		m_texture_storage;
	storage<d3d9effect*>		m_effect_storage;
	storage<d3d9vertexbuffer*>	m_vertexbuffer_storage;
	storage<d3d9indexbuffer*>	m_indexbuffer_storage;
	//---------------
	DWORD					m_dwBackgroundColor;
	renderer_create_param	m_create_param;

	IDirect3DSurface9*		m_backup_rendertarget[16];
};

typedef sptr<d3d9renderdevice> d3d9renderer_ptr;

}