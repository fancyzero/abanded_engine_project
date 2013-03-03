#pragma once

namespace mania
{
class d3d9vertex_declaration : public vertex_declaration
{
	friend class d3d9renderdevice;
public:
	d3d9vertex_declaration( void );
	~d3d9vertex_declaration( void );
	DWORD destroy();
protected:
	IDirect3DVertexDeclaration9*	m_d3dvertexdecl9;
};
}