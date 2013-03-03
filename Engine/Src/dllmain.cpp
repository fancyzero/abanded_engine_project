// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

MANIA_IMPLEMENT_CLASS( mania::effect_pool );

void register_all_class()
{
	MANIA_REGISTER_CLASS( mania::node );
	MANIA_REGISTER_CLASS( mania::tnode );
	MANIA_REGISTER_CLASS( mania::renderable );
	MANIA_REGISTER_CLASS( mania::staticmesh_renderable );
	MANIA_REGISTER_CLASS( mania::skeletalmesh_renderable );
	MANIA_REGISTER_CLASS( mania::effect_pool );
	MANIA_REGISTER_CLASS( mania::viewinfo );
	MANIA_REGISTER_CLASS( mania::camera );
	MANIA_DEFINE_PARENT_CLASS( mania::node, mania::object );
	MANIA_DEFINE_PARENT_CLASS( mania::renderable, mania::tnode );
	MANIA_DEFINE_PARENT_CLASS( mania::staticmesh_renderable, mania::renderable );
	MANIA_DEFINE_PARENT_CLASS( mania::skeletalmesh_renderable, mania::renderable );
	MANIA_DEFINE_PARENT_CLASS( mania::tnode, mania::node );
	MANIA_DEFINE_PARENT_CLASS( mania::effect_pool, mania::object );
	MANIA_DEFINE_PARENT_CLASS( mania::viewinfo, mania::object );
	MANIA_DEFINE_PARENT_CLASS( mania::camera, mania::tnode );
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

