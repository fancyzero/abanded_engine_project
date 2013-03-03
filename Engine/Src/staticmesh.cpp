#include "StdAfx.h"
#include "mesh_data_defines.h"
#include "staticmesh.h"
#include "collada_loader.h"

namespace mania
{

	staticmesh::staticmesh( void )
	{
	}

	staticmesh::~staticmesh( void )
	{
	}

	gresult staticmesh::load( const string& url )
	{
		return load_xml( convert_to_xmlurl( url ).c_str() );
	}

	int calc_triangle_component_stride_by_semantic( const string& sem, xmlfile::node& node )
	{
		if (( sem == L"VERTEX" ) || ( sem == L"NORMAL" ) )
			return 1;
		int set = 1;
		const TCHAR* set_attr = node.get_attribute( L"set" );
		if ( set_attr )
			set = _wtoi( set_attr );
		if (( sem == L"TEXCOORD" ) || ( sem == L"TEXTANGENT" ) || ( sem == L"TEXBINORMAL" ) )
			return set;
		return 0;
	}

	gresult staticmesh::load_xml( const TCHAR* url )
	{
		gresult gr;
		collada_loader cl;
		raw_mesh_data raw_data;
		gr = cl.build_raw_data( url );
		if ( failed( gr ) )
			return gr;

		return build_render_data( &cl.get_raw_data() );
	}

	gresult staticmesh::build_render_data( raw_mesh_data* data )
	{
		for ( size_t rsm = 0; rsm < data->submeshes.size(); rsm++ )
		{
			raw_mesh_data::submesh& rawsm = data->submeshes[rsm];
			submesh* sm_ref = new submesh();
			m_submeshes.push_back( sm_ref );
			// mark same vert
			std::vector<std::vector<unsigned int> > grouped_by_pos;
			std::vector<unsigned int> ib;
			std::vector<math::vector3> vb_pos;
			grouped_by_pos.reserve(rawsm.position_src.size());

			for ( std::vector<raw_mesh_data::submesh::material_record>::iterator matrec_it = rawsm.matrials.begin(); matrec_it != rawsm.matrials.end(); ++matrec_it )
			{
				grouped_by_pos.resize(rawsm.position_src.size());
				unsigned int vert_index = 0;
				unsigned int vert_index_max = matrec_it->start + matrec_it->length;
				mania::submesh::part part;
				part.m_base_vertexindex = ( unsigned int )vb_pos.size();
				part.m_startindex = matrec_it->start;                         

				for ( unsigned int i = matrec_it->start; i < vert_index_max; i++ )
				{
					grouped_by_pos[rawsm.verts[i].id[0]].push_back(i);
				}
				for ( unsigned int i = matrec_it->start; i < vert_index_max; i++ )
				{
					if ( rawsm.verts[i].vert_index == 0xffffffff )
					{
						rawsm.verts[i].vert_index = vert_index;
						vb_pos.push_back( rawsm.position_src[rawsm.verts[i].id[0]] );
						std::vector<unsigned int>& gbp_n = grouped_by_pos[rawsm.verts[i].id[0]];
						for ( unsigned int j = 0; j < gbp_n.size(); j++ )
						{//for ( unsigned int j = i + 1; j < vert_index_max; j++ )						
							if (  rawsm.verts[gbp_n[j]].vert_index == 0xffffffff )
							{
								bool same = true;
								for ( unsigned int w = 0; w < rawsm.vert_stride; w++ )
									if ( rawsm.verts[i].id[w] != rawsm.verts[gbp_n[j]].id[w] )
									{
										same = false;
										break;
									}
									if ( same )
										rawsm.verts[gbp_n[j]].vert_index = rawsm.verts[i].vert_index;
							}						
						}
						vert_index++;
					}
					ib.push_back( rawsm.verts[i].vert_index );
				}
				part.m_num_vertices = ( unsigned int )( vb_pos.size() - part.m_base_vertexindex );
				part.m_min_index = 0;
				part.m_effect_index = matrec_it->matid;
				part.m_primitive_count = matrec_it->length / 3;
				sm_ref->m_parts.push_back( part );
			}
			math::vector3* p1 = new math::vector3[vb_pos.size()];
			sptr<mania::buffer> vb_pos_buffer = new buffer(( void* )p1, ( unsigned int )vb_pos.size()*sizeof( math::vector3 ) );
			for ( unsigned int i = 0; i < vb_pos.size(); i++ )
				p1[i] = vb_pos[i];
			void* p2 = NULL;
			bool use_16bit_ib = ( ib.size() <= 0xffff );
			if ( !use_16bit_ib )
			{
				p2 = new unsigned int[ib.size()];
				for ( unsigned int i = 0; i < ib.size(); i++ )
					(( unsigned int* )p2 )[i] = ib[i];
			}
			else
			{
				p2 = new unsigned short[ib.size()];
				for ( unsigned int i = 0; i < ib.size(); i++ )
					(( unsigned short* )p2 )[i] = ib[i];
			}

			///
			///create vertexbuffer
			///
			vertexbuffer::desc_t desc( vb_pos_buffer->get_size(), usage_none, pool_managed, sizeof( float ) * 3 );
			render_task *newtask = new rendertask_create_vertexbuffer(
				get_core()->get_rendering_thread(),
				desc, &sm_ref->m_vertexbuffer, vb_pos_buffer );
			get_core()->get_rendering_thread()->AddRenderTask( newtask );
			///
			///create indexbuffer
			///
			sptr<mania::buffer> ib_buffer = new buffer( p2, ( unsigned int )ib.size() *( use_16bit_ib ? sizeof( unsigned short ) : sizeof( unsigned int ) ) );
			indexbuffer::desc_t desc_i( ib_buffer->get_size(), use_16bit_ib ?fmt_index16 : fmt_index32, usage_none, pool_managed );		
			newtask = new rendertask_create_indexbuffer(
				get_core()->get_rendering_thread(),
				desc_i, &sm_ref->m_indexbuffer, ib_buffer );
			get_core()->get_rendering_thread()->AddRenderTask( newtask );
			///
			///create vertex declaration
			///
			std::vector<vertexelem_def> ve_def;
			vertexelem_def def( 0, 0, decltype_float3, declmethod_default, declusage_position, 0 );
			ve_def.push_back( def );
			newtask = new rendertask_create_vertexdeclearation(
				get_core()->get_rendering_thread(),
				ve_def, &sm_ref->m_vertex_declaration );
			get_core()->get_rendering_thread()->AddRenderTask( newtask );
			sm_ref->m_effects.resize(rawsm.matrials.size());
			///
			///create effects
			///
			for( unsigned int i = 0; i < rawsm.matrials.size(); i++ )
			{
				ENQUEUE_RENDER_TASK_BEGIN_2PARAM( loadeffect, sptr<effect>*, effect2create, string, url2load )
					mania::get_core()->get_rendering_thread()->get_renderdevice()->create_effect_from_url( L"simplemesheff" , url2load, *( effect2create ) );
				return grc_succeeded;
				ENQUEUE_RENDER_TASK_END_2PARAM( loadeffect, &sm_ref->m_effects[i], L"simple.fx" );
			}
		}
		return grc_succeeded;
	}
}