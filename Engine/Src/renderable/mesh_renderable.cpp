#include "StdAfx.h"
#include "..\sg\renderable.h"
#include <d3dx9.h>
namespace mania
{	
	class draw_mesh_task : public mania::render_task
	{
	public:
		draw_mesh_task( mania::rendering_thread* rt, sptr<mesh> mesh, const matrix44& world_transform , const matrix44& vp_transform)
			: render_task( rt ) { m_mesh = mesh; m_world_transform = world_transform; m_vp_transform = vp_transform; }

		virtual gresult setup_renderstate( sptr<submesh> sm )
		{
			return grc_succeeded;
		}

		virtual DWORD execute()
		{
			UINT passes;
			for ( UINT subm = 0; subm < m_mesh->m_submeshes.size(); subm++ )
			{
				sptr<submesh> sub = m_mesh->m_submeshes[subm];
				gresult gr;
				gr = m_rendering_thread->get_renderdevice()->set_vertexdecl( sub->m_vertex_declaration );
				gr = m_rendering_thread->get_renderdevice()->set_texture( 0, NULL );
				gr = m_rendering_thread->get_renderdevice()->set_vertexbuffer( 0, sub->m_vertexbuffer_pos );
				//gr = m_rendering_thread->get_renderdevice()->set_vertexbuffer( 1, sub->m_vertexbuffer );
				m_rendering_thread->get_renderdevice()->set_indexbuffer( 0, sub->m_indexbuffer );
				sptr<effect> eff;
				foreach( std::vector< sptr<effect> >, eff, sub->m_effects )
				{
					//TODO : get string from pool
					//m_world_transform._11 = 11;
					//m_world_transform._12 = 12;
					//m_world_transform._13 = 13;
					//m_world_transform._14 = 14;
					//m_world_transform._21 = 21;
					//m_world_transform._22 = 22;
					//m_world_transform._23 = 23;
					//m_world_transform._24 = 24;
					//m_world_transform._31 = 31;
					//m_world_transform._32 = 32;
					//m_world_transform._33 = 33;
					//m_world_transform._34 = 34;
					//m_world_transform._41 = 41;
					//m_world_transform._42 = 42;
					//m_world_transform._43 = 43;
					//m_world_transform._44 = 44;
					( *eff )->set_param( "world_xf", m_world_transform );
					( *eff )->set_param( "vp_xf", m_vp_transform );
				}
				return_if_failed( setup_renderstate( sub ) );
				for ( UINT partn = 0; partn < sub->m_parts.size(); partn++ )
				{
					submesh::part& part = sub->m_parts[partn];
					if ( sub->m_effects[part.m_effect_index]->begin( passes, 0 ) == grc_succeeded )
					{
						for ( UINT p = 0; p < passes; p++ )
						{
							sub->m_effects[part.m_effect_index]->begin_pass( p );
							m_rendering_thread->get_renderdevice()->draw_indexedprimitive(
								primitive_trianglelist,
								part.m_base_vertexindex,
								part.m_min_index,
								part.m_num_vertices,
								part.m_startindex,
								part.m_primitive_count );
							sub->m_effects[part.m_effect_index]->end_pass();
						}
						sub->m_effects[sub->m_parts[partn].m_effect_index]->end();
					}
				}
			}
			return grc_succeeded;
		}

		sptr<mesh>	m_mesh;
		matrix44			m_world_transform;
		matrix44			m_vp_transform;
	};

	class draw_staticmesh_task : public mania::render_task
	{
	public:
		draw_staticmesh_task( mania::rendering_thread* rt, sptr<staticmesh> sm, const matrix44& world_transform , const matrix44& vp_transform)
			: render_task( rt ) { m_sm = sm; m_world_transform = world_transform; m_vp_transform = vp_transform; }

		DWORD execute()
		{
			UINT passes;
			for ( UINT subm = 0; subm < m_sm->m_submeshes.size(); subm++ )
			{
				sptr<submesh> sub = m_sm->m_submeshes[subm];
				gresult gr;
				gr = m_rendering_thread->get_renderdevice()->set_vertexdecl( sub->m_vertex_declaration );
				gr = m_rendering_thread->get_renderdevice()->set_texture( 0, NULL );
				gr = m_rendering_thread->get_renderdevice()->set_vertexbuffer( 0, sub->m_vertexbuffer );
				m_rendering_thread->get_renderdevice()->set_indexbuffer( 0, sub->m_indexbuffer );
				sptr<effect> eff;
				foreach( std::vector< sptr<effect> >, eff, sub->m_effects )
				{
					//TODO : get string from pool
					( *eff )->set_param( "world_xf", m_world_transform );
					( *eff )->set_param( "vp_xf", m_vp_transform );
				}
				for ( UINT partn = 0; partn < sub->m_parts.size(); partn++ )
				{
					submesh::part& part = sub->m_parts[partn];
					if ( sub->m_effects[part.m_effect_index]->begin( passes, 0 ) == grc_succeeded )
					{
						for ( UINT p = 0; p < passes; p++ )
						{
							sub->m_effects[part.m_effect_index]->begin_pass( p );
							m_rendering_thread->get_renderdevice()->draw_indexedprimitive(
								primitive_trianglelist,
								part.m_base_vertexindex,
								part.m_min_index,
								part.m_num_vertices,
								part.m_startindex,
								part.m_primitive_count );
							sub->m_effects[part.m_effect_index]->end_pass();
						}
						sub->m_effects[sub->m_parts[partn].m_effect_index]->end();
					}
				}
			}
			return grc_succeeded;
		}

		sptr<staticmesh>	m_sm;
		matrix44			m_world_transform;
		matrix44			m_vp_transform;
	};


	class draw_skeletalmesh_task : public mania::draw_mesh_task
	{
	public:
		draw_skeletalmesh_task( mania::rendering_thread* rt, sptr<skeletalmesh> skmesh, const matrix44& world_transform , const matrix44& vp_transform ,
			const math::matrix44* world_matrix_array, UINT matrix_count )
			: draw_mesh_task( rt, skmesh, world_transform, vp_transform ),
			m_world_matrix_array( world_matrix_array ),
			m_matrix_count ( matrix_count )
		{
		}

		virtual gresult setup_renderstate( sptr<submesh> sub )
		{
			//transport bones pose matrix to vertex shader
			foreach( std::vector< sptr<effect> >, eff, sub->m_effects )
			{
				( *eff )->set_param( "world_matrix_array", m_world_matrix_array, m_matrix_count );
			}
			return grc_succeeded;
		}

		const math::matrix44*	m_world_matrix_array;
		int						m_matrix_count;

	};



	///staticmesh renderable
	MANIA_IMPLEMENT_CLASS( staticmesh_renderable );
	gresult staticmesh_renderable::render(mania::render_context &render_context)
	{
			//draw_skeletalmesh_task* newtask = new draw_skeletalmesh_task( get_core()->get_rendering_thread(), ( *it2 )->get_mesh(), ( *it2 )->get_local_transform().get_matrix(), m_viewinfo.get_view_proj() );
			//get_core()->get_rendering_thread()->AddRenderTask( newtask );
		return grc_failed;
	}

	//skeletalmesh_renderable	
	MANIA_IMPLEMENT_CLASS( skeletalmesh_renderable );
	skeletalmesh_renderable::skeletalmesh_renderable()
	: renderable(),m_current_matrix_array(NULL)
	{		
		MANIA_CLASS_CONSTRUCTOR_INCLUDE( skeletalmesh_renderable );

	}
	skeletalmesh_renderable::~skeletalmesh_renderable()
	{
		m_poses.clear();
	}
	void skeletalmesh_renderable::set_mesh( sptr<skeletalmesh> sm )
	{
		check(sm);
		//TODO: keep pose as unchanged as possible 
		//clear old pose tree
		m_skeletalmesh = sm;
		m_poses.clear();
		m_poses.resize( sm->get_bone_count() );
		for ( int i = 0; i < sm->get_bone_count(); i++ )
			m_poses[i] = new skeleton_posenode();		
		for ( int i = 0; i < sm->get_bone_count(); i++ )
		{
			m_poses[i]->m_index = (int)i;
			m_poses[i]->get_local_transform().set_matrix( sm->get_bone(i).local_matrix );
			for( int j = 0; j < sm->get_bone(i).children.size(); j++ )
			{
				m_poses[i]->attach(m_poses[sm->get_bone(i).children[j]]);
			}
		}
		//for ( int i = 0; i < m_poses.size(); i++ )
		//{
		//	skeleton_posenode* parent_node = m_poses[i].get_parent();
		//	if ( parent_node )
		//	{
		//		math::matrix44 inv_parent_mat = sm->get_bone(parent_node->m_index).offset_matrix;
		//		inv_parent_mat.inverse();

		//		m_poses[i].get_local_transform().set_matrix( sm->get_bone(m_poses[i].m_index).offset_matrix * inv_parent_mat );
		//	}
		//}
		//TODO: thread safe
		if ( m_current_matrix_array != NULL )
			delete[] m_current_matrix_array; 
		m_current_matrix_array = new math::matrix44[m_poses.size()];
	}

	gresult skeletalmesh_renderable::render(mania::render_context &rc)
	{
		float r = sin(GetTickCount()/1000.0f)*0.01f;
		static int iii = 13;
		//m_poses[0].get_local_transform().set_rotation( 0, 3.1415926f/2, 0 );
		//m_poses[13].get_local_transform().set_rotation( 0, r, 0 );
		
		for ( int i = 0; i < m_poses.size(); i++ )
		{
			math::matrix44 matrot;
			math::build_matrix_rotation_axis( &matrot, axis_y, r ); 
			m_poses[i]->get_local_transform().set_matrix( matrot * m_poses[i]->get_local_transform().get_matrix() );
		}
		for ( int i = 0; i < m_poses.size(); i++ )
		{
			
			int index = m_skeletalmesh->get_bone(m_poses[i]->m_index).index;
			const math::matrix44& bind_shape	= m_skeletalmesh->get_bind_shape_matrix();
			math::matrix44 bone_offset = m_skeletalmesh->get_bone(m_poses[i]->m_index).offset_matrix;
			math::matrix44 bone_world	= m_poses[i]->get_world_transform().get_matrix();
			
			m_current_matrix_array[index] = bind_shape * bone_offset * bone_world;
			math::matrix44 mattmp = bone_offset * bone_world;
			GetTickCount();
		}

		draw_skeletalmesh_task* newtask = new draw_skeletalmesh_task( rc.m_rendering_thread, m_skeletalmesh, get_local_transform().get_matrix(), rc.m_viewinfo.get_view_proj(), m_current_matrix_array, (UINT)m_poses.size() );
		return rc.m_rendering_thread->AddRenderTask( newtask );
	}
}