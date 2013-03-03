// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "..\..\engine\src\plugins\renderdevice\renderdevice.h"
#include "d3d9renderdevice.h"
#include "d3d9effect.h"
#include "d3d9texture.h"
#include "d3d9indexbuffer.h"
#include "D3D9VertexBuffer.h"
#include "d3d9font.h"

void register_all_class()
{
	MANIA_REGISTER_CLASS( mania::d3d9effect_res_base );
	MANIA_REGISTER_CLASS( mania::d3dfont_res_base );
	MANIA_REGISTER_CLASS( mania::d3d9indexbuffer_res_base );
	MANIA_REGISTER_CLASS( mania::d3d9texture_res_base );
	MANIA_REGISTER_CLASS( mania::d3d9vertexbuffer_res_base );
	MANIA_DEFINE_PARENT_CLASS( mania::d3d9effect_res_base,		mania::object );
	MANIA_DEFINE_PARENT_CLASS( mania::d3dfont_res_base,			mania::object );
	MANIA_DEFINE_PARENT_CLASS( mania::d3d9indexbuffer_res_base,	mania::object );
	MANIA_DEFINE_PARENT_CLASS( mania::d3d9texture_res_base,		mania::object );
	MANIA_DEFINE_PARENT_CLASS( mania::d3d9vertexbuffer_res_base, mania::object );
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	switch ( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:
		register_all_class();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}




