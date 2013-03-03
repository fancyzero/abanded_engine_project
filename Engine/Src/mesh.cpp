#include "StdAfx.h"
#include "mesh.h"

namespace mania
{
submesh& submesh::operator = ( const submesh& a_submesh )
{
	m_parts					= a_submesh.m_parts;
	m_vertex_declaration	= a_submesh.m_vertex_declaration;
	m_vertexbuffer			= a_submesh.m_vertexbuffer;
	m_indexbuffer			= a_submesh.m_indexbuffer;
	m_effects				= a_submesh.m_effects;
	return *this;
}
mesh::mesh( void )
{
}

mesh::~mesh( void )
{
}

}