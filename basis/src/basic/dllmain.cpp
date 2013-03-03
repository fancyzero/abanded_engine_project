#include "stdafx.h"

void register_all_class()
{
	MANIA_DEFINE_PARENT_CLASS( mania::buffer, mania::object );
	MANIA_DEFINE_PARENT_CLASS( mania::resource, mania::object );
	MANIA_REGISTER_CLASS( mania::object );
	MANIA_REGISTER_CLASS( mania::buffer );
	MANIA_REGISTER_CLASS( mania::resource );
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	switch ( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:
		if ( !mania::identifier::cs.initialized() )
			mania::identifier::cs.create( NULL );
		register_all_class();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}