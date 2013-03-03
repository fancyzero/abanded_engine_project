#pragma once

namespace mania
{

class rendering_thread;
class texture;
class effect;
class vertexbuffer;
class indexbuffer;
class vertex_declaration;

class MANIA_ENGINE_API renderdevice : public object
{
public:
	renderdevice( void ) {};
	virtual ~renderdevice( void ) {};

	virtual DWORD Create( renderer_create_param )	= 0;
	virtual DWORD Destroy()				= 0;

	//scene rendering
	virtual DWORD begin_frame()			= 0;
	virtual DWORD end_frame()			= 0;

	//RenderStates
	virtual void SetBackgroundColor( DWORD dwColor ) = 0;

	//Threading
public:
	virtual void SetHostThread( rendering_thread* pThread ) = 0;

	//texture
	virtual DWORD create_texture( const string& name, int w, int h, int mip_count, int usage, int format, int pool, sptr<texture>& texLoaded ) = 0;
	virtual DWORD create_texture_from_url( const string& name, const string& url, int mip_count,
	                                       int usage, int format, int pool, sptr<texture>& texLoaded ) = 0;
	virtual DWORD create_texture_from_mem( const string& name, const void* dataaddr, DWORD datasize,
	                                       int mip_count, int usage, int format, int pool, sptr<texture>& texLoaded ) = 0;
	virtual DWORD destroy_texture( texture* spTex ) = 0;

	//effect
	virtual DWORD create_effect_from_url( const string&name, const string& url, sptr<effect>& effcreated ) = 0;
	virtual DWORD destroy_effect( effect* spTex ) = 0;
	//rendering
	virtual DWORD draw_indexedprimitive( eprimitivetype type, int base_index, UINT min_index, UINT num_vertices, int start_index,  UINT count ) = 0;
	virtual DWORD draw_indexedprimitive_mem( void* vertex_data, UINT vertex_stride, void* index_data, eformat index_format, eprimitivetype type, UINT min_index, UINT num_vertices,  UINT count ) = 0;

	//vb ib
	virtual DWORD create_vertexbuffer( const vertexbuffer::desc_t& desc, sptr<vertexbuffer>& vb_created ) = 0;
	virtual DWORD create_indexbuffer( const indexbuffer::desc_t& desc, sptr<indexbuffer>& ib_created ) = 0;

	//vertex decl
	virtual DWORD create_vertexddecl( const std::vector<vertexelem_def>& elemdefs, sptr<vertex_declaration>& vd ) = 0;
	virtual DWORD set_vertexdecl( sptr<vertex_declaration> vd ) = 0;

	//states
	virtual DWORD set_texture( UINT index, sptr<texture> tex ) = 0;
	virtual DWORD set_vertexbuffer( UINT index, sptr<vertexbuffer> vb ) = 0;
	virtual DWORD set_indexbuffer( UINT index, sptr<indexbuffer> ib ) = 0;
	virtual DWORD set_rendertarget( UINT index, sptr<texture> texture ) = 0;
	virtual DWORD restore_rendertarget( UINT index ) = 0;

	//misc
	virtual	DWORD reset( renderer_create_param param ) = 0;
public:
	virtual renderer_create_param get_create_param()  = 0;
};

typedef sptr<renderdevice> renderdevice_ptr;
}