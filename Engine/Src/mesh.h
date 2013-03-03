#pragma once

namespace mania
{
	struct MANIA_ENGINE_API submesh : public ref_counted
{
	struct MANIA_ENGINE_API part
	{
		unsigned int				m_base_vertexindex;
		unsigned int				m_num_vertices;
		unsigned int				m_startindex;
		unsigned int				m_min_index;
		unsigned int				m_effect_index;
		unsigned int				m_primitive_count;
	};
	submesh& operator = ( const submesh& a_submesh );

	std::vector<part>				m_parts;
	sptr<vertex_declaration>		m_vertex_declaration_pos;
	sptr<vertex_declaration>		m_vertex_declaration;
	sptr<vertexbuffer>				m_vertexbuffer_pos;
	sptr<vertexbuffer>				m_vertexbuffer;
	sptr<indexbuffer>				m_indexbuffer;
	std::vector< sptr<effect> >		m_effects;

};

class MANIA_ENGINE_API mesh : public resource
{
public:
	mesh( void );
	~mesh( void );

	virtual gresult load( const string& url ) = 0;

	std::vector<sptr<submesh> >		m_submeshes;

};
}