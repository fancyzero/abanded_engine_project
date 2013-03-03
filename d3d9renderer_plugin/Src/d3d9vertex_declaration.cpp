#include "StdAfx.h"
#include "d3d9renderdevice.h"
#include "d3d9effect.h"
#include "d3d9texture.h"
#include "d3d9vertex_declaration.h"

namespace mania
{
d3d9vertex_declaration::d3d9vertex_declaration( void )
		: m_d3dvertexdecl9( NULL ) {}


d3d9vertex_declaration::~d3d9vertex_declaration( void )
{
	destroy();
}

DWORD d3d9vertex_declaration::destroy()
{
	ENQUEUE_RENDER_TASK_BEGIN_1PARAM( task_destroy_decl, IDirect3DVertexDeclaration9*, decl )
		safe_Release( decl );
		return grc_succeeded;
	ENQUEUE_RENDER_TASK_END_1PARAM( task_destroy_decl, m_d3dvertexdecl9 )		
	
	return grc_succeeded;
}
}