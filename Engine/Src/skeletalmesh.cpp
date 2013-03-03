#include "StdAfx.h"
#include "mesh_data_defines.h"
#include "skeletalmesh.h"
#include "collada_loader.h"
namespace mania
{
	skeletalmesh::skeletalmesh(void)
	{
	}

	skeletalmesh::~skeletalmesh(void)
	{
	}

	gresult skeletalmesh::load( const string& url )
	{
		gresult gr = load_from_bin( url.c_str() );
		if ( failed(gr) )
			gr = load_from_xml( convert_to_xmlurl(url).c_str() );
		return gr;
	}

	gresult skeletalmesh::load_from_bin( const TCHAR* filename )
	{
		return grc_failed;		
	}
	gresult skeletalmesh::load_from_xml( const TCHAR* filename )
	{
		gresult gr;
		collada_loader cl;
		gr = cl.build_raw_data( filename );
		if ( failed( gr ) )
			return gr;

		return build_render_data( &cl.get_raw_data() );
	}
	 
	void fill_bone_influences( skeletalmesh::skeletal_vertex_raw& v, const raw_mesh_data::submesh::bone_influnce& inf )
	{
		unsigned char bone_id[4]={0,0,0,0};
		float		  bone_weight[4]={ 0,0,0,0 };
		unsigned int  min_id = 0;
		float		  tiny_enough = 0.001f;
		for ( unsigned int i = 0; i < inf.size(); i++ )
		{
			float cur_weight = inf[i].second;
			if ( cur_weight < tiny_enough )
				cur_weight = 0.0f;
			if ( cur_weight > bone_weight[min_id] )
			{
				bone_weight[min_id] = cur_weight;
				bone_id[min_id] = inf[i].first;
				int j = 0;
				while( j < 4 )
				{
					if ( bone_weight[j] < bone_weight[min_id] )
						min_id = j;
					j++;
				}
			}
		}
		//normalize weights
		float sum = bone_weight[0] + 
			bone_weight[1] + 
			bone_weight[2] + 
			bone_weight[3];
		v.bone_id[0] = bone_id[0];
		v.bone_id[1] = bone_id[1];
		v.bone_id[2] = bone_id[2];
		v.bone_id[3] = bone_id[3];

		v.bone_weight[0] = bone_weight[0] / sum;
		v.bone_weight[1] = bone_weight[1] / sum;
		v.bone_weight[2] = bone_weight[2] / sum;
		v.bone_weight[3] = bone_weight[3] / sum;
	}

	gresult skeletalmesh::build_render_data( struct raw_mesh_data* data )
	{
		for ( size_t rsm = 0; rsm < data->submeshes.size(); rsm++ )
		{
			raw_mesh_data::submesh& rawsm = data->submeshes[rsm];
			submesh* sm_ref = new submesh();
			m_submeshes.push_back( sm_ref );
			// mark same vert
			std::vector<std::vector<unsigned int> > grouped_by_pos;
			std::vector<unsigned int> ib;
			std::vector<skeletal_vertex_raw> vb_raw;
			grouped_by_pos.reserve(rawsm.position_src.size());

			unsigned int pos_src_id = 0;
			unsigned int uv_src_id = 1;
			unsigned int normal_src_id = 2;

			rawsm.get_source_index( L"position", pos_src_id );
			rawsm.get_source_index( L"uv", uv_src_id );
			rawsm.get_source_index( L"normal", normal_src_id );

			for ( std::vector<raw_mesh_data::submesh::material_record>::iterator matrec_it = rawsm.matrials.begin(); matrec_it != rawsm.matrials.end(); ++matrec_it )
			{
				grouped_by_pos.resize(rawsm.position_src.size());
				unsigned int vert_index = 0;
				unsigned int vert_index_max = matrec_it->start + matrec_it->length;
				mania::submesh::part part;
				part.m_base_vertexindex = ( unsigned int )vb_raw.size();
				part.m_startindex = matrec_it->start;                         


				for ( unsigned int i = matrec_it->start; i < vert_index_max; i++ )
				{
					grouped_by_pos[rawsm.verts[i].id[pos_src_id]].push_back(i);
				}
				for ( unsigned int i = matrec_it->start; i < vert_index_max; i++ )
				{
					if ( rawsm.verts[i].vert_index == 0xffffffff )
					{
						rawsm.verts[i].vert_index = vert_index;
						skeletal_vertex_raw tmp_v_raw;
						tmp_v_raw.pos = rawsm.position_src[rawsm.verts[i].id[pos_src_id]];
						tmp_v_raw.uv = rawsm.uv_src[rawsm.verts[i].id[uv_src_id]];
						fill_bone_influences( tmp_v_raw, rawsm.bone_influences[rawsm.verts[i].id[pos_src_id]] );
						vb_raw.push_back( tmp_v_raw );
						std::vector<unsigned int>& gbp_n = grouped_by_pos[rawsm.verts[i].id[pos_src_id]];
						for ( unsigned int j = 0; j < gbp_n.size(); j++ )
						{					
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
				part.m_num_vertices = ( unsigned int )( vb_raw.size() - part.m_base_vertexindex );
				part.m_min_index = 0;
				part.m_effect_index = matrec_it->matid;
				part.m_primitive_count = matrec_it->length / 3;
				sm_ref->m_parts.push_back( part );
			}


			skeletal_vertex_posonly* vb_pos = new skeletal_vertex_posonly[vb_raw.size()];
			skeletal_vertex* vb_ = new skeletalmesh::skeletal_vertex[vb_raw.size()];
			
			sptr<mania::buffer> vb_buffer_pos = new buffer(( void* )vb_pos, ( unsigned int )vb_raw.size()*sizeof( skeletal_vertex_posonly ) );
			sptr<mania::buffer> vb_buffer_ = new buffer(( void* )vb_, ( unsigned int )vb_raw.size()*sizeof( skeletal_vertex ) );

			for ( unsigned int i = 0; i < vb_raw.size(); i++ )
			{
				vb_pos[i].pos = vb_raw[i].pos;
				vb_pos[i].bone_id[0] = vb_raw[i].bone_id[0];
				vb_pos[i].bone_id[1] = vb_raw[i].bone_id[1];
				vb_pos[i].bone_id[2] = vb_raw[i].bone_id[2];
				vb_pos[i].bone_id[3] = vb_raw[i].bone_id[3];
				vb_pos[i].bone_weight[0] = vb_raw[i].bone_weight[0];
				vb_pos[i].bone_weight[1] = vb_raw[i].bone_weight[1];
				vb_pos[i].bone_weight[2] = vb_raw[i].bone_weight[2];
				vb_pos[i].bone_weight[3] = vb_raw[i].bone_weight[3];

				vb_[i].uv = vb_raw[i].uv;
			}
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
			///create vertexbuffer position only
			///
			vertexbuffer::desc_t desc_posonly( vb_buffer_pos->get_size(), usage_none, pool_managed, sizeof( skeletal_vertex_posonly ) );
			render_task *newtask = new rendertask_create_vertexbuffer(
				get_core()->get_rendering_thread(),
				desc_posonly, &sm_ref->m_vertexbuffer_pos, vb_buffer_pos );
			get_core()->get_rendering_thread()->AddRenderTask( newtask );
			///
			///create vertexbuffer
			///
			vertexbuffer::desc_t desc( vb_buffer_->get_size(), usage_none, pool_managed, sizeof( skeletal_vertex ) );
			newtask = new rendertask_create_vertexbuffer(
				get_core()->get_rendering_thread(),
				desc, &sm_ref->m_vertexbuffer, vb_buffer_ );
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
			/// pos only
			
			std::vector<vertexelem_def> ve_def;
			//ve_def.clear();
			//ve_def.push_back( vertexelem_def( 0, 0, decltype_float3, declmethod_default, declusage_position, 0 ) );
			//
			//newtask = new rendertask_create_vertexdeclearation(
			//	get_core()->get_rendering_thread(),
			//	ve_def, &sm_ref->m_vertex_declaration_pos );
			//get_core()->get_rendering_thread()->AddRenderTask( newtask );
			/// other
			ve_def.clear();
			ve_def.push_back( vertexelem_def( 0, 0, decltype_float3, declmethod_default, declusage_position, 0 ) );
			ve_def.push_back( vertexelem_def( 0, 12, decltype_ubyte4, declmethod_default, declusage_blendindices, 0 ) );
			ve_def.push_back( vertexelem_def( 0, 12+4, decltype_float4, declmethod_default, declusage_blendweight, 0 ) );
			//ve_def.push_back( vertexelem_def( 1, 0, decltype_float2, declmethod_default, declusage_texcoord, 0 ) );
			newtask = new rendertask_create_vertexdeclearation(
				get_core()->get_rendering_thread(),
				ve_def, &sm_ref->m_vertex_declaration );
			get_core()->get_rendering_thread()->AddRenderTask( newtask );
			
			///
			///create effects
			///
			sm_ref->m_effects.resize(rawsm.matrials.size());
			for( unsigned int i = 0; i < rawsm.matrials.size(); i++ )
			{
				ENQUEUE_RENDER_TASK_BEGIN_2PARAM( loadeffect, sptr<effect>*, effect2create, string, url2load )
					mania::get_core()->get_rendering_thread()->get_renderdevice()->create_effect_from_url( L"skinnedmesheff" , url2load, *( effect2create ) );
				return grc_succeeded;
				ENQUEUE_RENDER_TASK_END_2PARAM( loadeffect, &sm_ref->m_effects[i], L"simple.fx" );
			}
		}
		//bones
		m_bones.resize( data->bones.size() );
		for( size_t i = 0; i < m_bones.size(); i++ )
		{
			m_bones[i].name = data->bones[i].name;
			m_bones[i].sid = data->bones[i].sid;
			m_bones[i].index = data->bones[i].index;
			m_bones[i].local_matrix = data->bones[i].local_matrix;
			m_bones[i].offset_matrix = data->bones[i].offset_matrix;
			m_bones[i].children = data->bones[i].children;
		}
		//bind shape
		m_bind_shape_matrix = data->bind_shape_matrix;
		return grc_succeeded;		
	}
}