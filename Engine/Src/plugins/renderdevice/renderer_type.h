#pragma once

namespace mania
{

enum eresourcetype
{
	rtype_surface = 1,
	rtype_volume = 2,
	rtype_texture = 3,
	rtype_volumetexture = 4,
	rtype_cubetexture = 5,
	rtype_vertexbuffer = 6,
	rtype_indexbuffer = 7,
};

enum epool
{
	pool_default	= 0,
	pool_managed	= 1,
	pool_systemmem	= 2,
	pool_scratch	= 3,
};

enum eprimitivetype
{
	primitive_pointlist = 1,
	primitive_linelist = 2,
	primitive_linestrip = 3,
	primitive_trianglelist = 4,
	primitive_trianglestrip = 5,
	primitive_trianglefan = 6,
	primitive_force_dword = 0x7fffffff,
};

enum eusage
{
	usage_none				 = 0,
	usage_rendertarget       = ( 0x001 ),
	usage_depthstencil       = ( 0x002 ),
	usage_dynamic            = ( 0x200 ),
	usage_writeonly          = ( 0x008 )
};

enum eformat
{
	fmt_rgb,
	fmt_argb,
	fmt_xrgb,
	fmt_argb_f16,
	fmt_index16,
	fmt_index32
};

typedef enum edecltype
{
	decltype_float1 = 0,
	decltype_float2 = 1,
	decltype_float3 = 2,
	decltype_float4 = 3,
	decltype_d3dcolor = 4,
	decltype_ubyte4 = 5,
	decltype_short2 = 6,
	decltype_short4 = 7,
	decltype_ubyte4n = 8,
	decltype_short2n = 9,
	decltype_short4n = 10,
	decltype_ushort2n = 11,
	decltype_ushort4n = 12,
	decltype_udec3 = 13,
	decltype_dec3n = 14,
	decltype_float16_2 = 15,
	decltype_float16_4 = 16,
	decltype_unused = 17,
};

typedef enum edeclmethod
{
	declmethod_default = 0,
	declmethod_partialu = 1,
	declmethod_partialv = 2,
	declmethod_crossuv = 3,
	declmethod_uv = 4,
	declmethod_lookup = 5,
	declmethod_lookuppresampled = 6,
};

typedef enum edeclusage
{
	declusage_position = 0,
	declusage_blendweight = 1,
	declusage_blendindices = 2,
	declusage_normal = 3,
	declusage_psize = 4,
	declusage_texcoord = 5,
	declusage_tangent = 6,
	declusage_binormal = 7,
	declusage_tessfactor = 8,
	declusage_positiont = 9,
	declusage_color = 10,
	declusage_fog = 11,
	declusage_depth = 12,
	declusage_sample = 13,
};

struct renderer_create_param
{
	UINT w;
	UINT h;
	UINT backbuffer_count;
	BOOL windowed;
	HWND host_window;
};

struct vertexelem_def
{
	UINT	stream;     // Stream index
	UINT	offset;     // Offset in the stream in bytes
	edecltype		type;       // Data type
	edeclmethod     process_method;     // Processing method
	edeclusage      usage;      // Semantics
	UINT    usage_index; // Semantic index
	vertexelem_def( UINT Astream, UINT Aoffset, edecltype Atype,
	                edeclmethod Aprocess_method, edeclusage Ausage, UINT Ausage_index )
			: stream( Astream ), offset( Aoffset ), type( Atype ),
			process_method( Aprocess_method ), usage( Ausage ), usage_index( Ausage_index )
	{
	}
};
}