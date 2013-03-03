#include "StdAfx.h"
#include "widget.h"
#include "drawer.h"

using namespace mania;

struct simple_vertex
{
	float x;
	float y;
	float z;
	float uv0[2];
	float uv1[2];
};

class drawwidgettask : public mania::render_task
{
public:
	sptr<widget> m_wdg;
	drawer*	m_drawer;

	drawwidgettask( mania::rendering_thread* rt )
			: render_task( rt ) {}

	DWORD execute()
	{
		UINT passes;
		m_rendering_thread->get_renderdevice()->set_vertexdecl( m_drawer->m_simple_vertexdecl );
		m_rendering_thread->get_renderdevice()->set_texture( 0, m_wdg->m_texture );
		m_rendering_thread->get_renderdevice()->set_vertexbuffer( 0, m_drawer->m_simple_vb );
		m_rendering_thread->get_renderdevice()->set_indexbuffer( 0, m_drawer->m_simple_ib );
		m_wdg->m_effect->set_param( "tex0", m_wdg->m_texture );
		m_wdg->m_effect->set_param( "time", ::GetTickCount() / 1000.0f );
		if ( m_wdg->m_effect->begin( passes, 0 ) == grc_succeeded )
		{
			for ( UINT p = 0; p < passes; p++ )
			{
				m_wdg->m_effect->begin_pass( p );
				m_rendering_thread->get_renderdevice()->draw_indexedprimitive( primitive_trianglelist, 0, 0, 4, 0, 2 );
				m_wdg->m_effect->end_pass();
			}
			m_wdg->m_effect->end();
		}
		return grc_succeeded;
	}
};

class draw_staticmesh_task : public mania::render_task
{
public:
	draw_staticmesh_task( mania::rendering_thread* rt, sptr<staticmesh> sm )
			: render_task( rt ) { m_sm = sm; }

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

	sptr<staticmesh> m_sm;
};

drawer::drawer( void )
{
}

drawer::~drawer( void )
{
}

DWORD drawer::init( sptr<renderdevice> renderdevice )
{
	simple_vertex vb;
	std::vector<mania::vertexelem_def> vd;
	m_renderdevice = renderdevice;
	vd.push_back( vertexelem_def( 0, ( UINT )(( char* )( &vb.x ) - ( char* )&vb ), decltype_float3, declmethod_default, declusage_position, 0 ) );
	vd.push_back( vertexelem_def( 0, ( UINT )(( char* )vb.uv0 - ( char* )&vb ), decltype_float2, declmethod_default, declusage_texcoord, 0 ) );
	vd.push_back( vertexelem_def( 0, ( UINT )(( char* )vb.uv1 - ( char* )&vb ), decltype_float2, declmethod_default, declusage_texcoord, 1 ) );
	class drawer_init_task : public render_task
	{
	public:
		drawer_init_task( rendering_thread* pRenderingThread, sptr<mania::vertex_declaration>& vertexdecl )
				: render_task( pRenderingThread ), m_vertexdecl( vertexdecl )
		{
		}
		std::vector<mania::vertexelem_def> m_vd;
		sptr<mania::vertex_declaration>& m_vertexdecl;
		sptr<vertexbuffer>*	m_pvb;
		sptr<indexbuffer>*	m_pib;

		DWORD execute()
		{
			vertexbuffer::desc_t vbdesc;
			vbdesc.size = sizeof( simple_vertex ) * 4;
			vbdesc.usage = usage_none;
			vbdesc.pool = pool_managed;
			vbdesc.stride = sizeof( simple_vertex );
			indexbuffer::desc_t  ibdesc;
			ibdesc.size = sizeof( short ) * 6;
			ibdesc.format = fmt_index16;
			ibdesc.usage = usage_none;
			ibdesc.pool = pool_managed;
			ibdesc.type = rtype_indexbuffer;
			m_rendering_thread->get_renderdevice()->create_vertexddecl( m_vd, m_vertexdecl );
			m_rendering_thread->get_renderdevice()->create_vertexbuffer( vbdesc, *m_pvb );
			m_rendering_thread->get_renderdevice()->create_indexbuffer( ibdesc, *m_pib );
			simple_vertex vb[4];
			short ib[6] = {0, 2, 1, 1, 2, 3};
			ZeroMemory( vb, sizeof vb );
			float x, y, w, h;
			w = 1024.f;
			h = 768.f;
			x = 0.0f;
			y = 0.0f;
			vb[0].x = x;
			vb[0].y = y;
			vb[0].z = 0;
			vb[0].uv0[0] = 0;
			vb[0].uv0[1] = 0;
			vb[1].x = x + w;
			vb[1].y = y;
			vb[1].z = 0;
			vb[1].uv0[0] = 1;
			vb[1].uv0[1] = 0;
			vb[2].x = x;
			vb[2].y = y + h;
			vb[2].z = 0;
			vb[2].uv0[0] = 0;
			vb[2].uv0[1] = 1;
			vb[3].x = x + w;
			vb[3].y = y + h;
			vb[3].z = 0;
			vb[3].uv0[0] = 1;
			vb[3].uv0[1] = 1;
			void* lockeddata;
			( *m_pvb )->lock( 0, 0, &lockeddata, 0 );
			memcpy( lockeddata, vb, sizeof( simple_vertex ) *4 );
			( *m_pvb )->unlock();
			( *m_pib )->lock( 0, 0, &lockeddata, 0 );
			memcpy( lockeddata, ib, sizeof( short ) *6 );
			( *m_pib )->unlock();
			return grc_succeeded;
		}
	};
	drawer_init_task* newtask = new drawer_init_task( get_core()->get_rendering_thread(), m_simple_vertexdecl );
	newtask->m_vd = vd;
	newtask->m_pvb = &m_simple_vb;
	newtask->m_pib = &m_simple_ib;
	get_core()->get_rendering_thread()->AddRenderTask( newtask );
	return grc_succeeded;
}

DWORD drawer::uninit()
{
	ENQUEUE_RENDER_TASK_BEGIN_1PARAM( task_1, sptr<vertex_declaration>, a )
	return a->destroy();
	ENQUEUE_RENDER_TASK_END_1PARAM( task_1, m_simple_vertexdecl );
	ENQUEUE_RENDER_TASK_BEGIN_1PARAM( task_1, sptr<indexbuffer>, a )
	return a->destroy();
	ENQUEUE_RENDER_TASK_END_1PARAM( task_1, m_simple_ib );
	ENQUEUE_RENDER_TASK_BEGIN_1PARAM( task_2, sptr<vertexbuffer>, a )
	return a->destroy();
	ENQUEUE_RENDER_TASK_END_1PARAM( task_2, m_simple_vb );
	return grc_succeeded;
}

DWORD drawer::draw_widget( sptr<widget> wdg )
{
	drawwidgettask* newtask = new drawwidgettask( get_core()->get_rendering_thread() );
	newtask->m_wdg = wdg;
	newtask->m_drawer = this;
	get_core()->get_rendering_thread()->AddRenderTask( newtask );
	return grc_succeeded;
}

DWORD drawer::draw_staticmesh( sptr<staticmesh> sm )
{
	draw_staticmesh_task* newtask = new draw_staticmesh_task( get_core()->get_rendering_thread(), sm );
	get_core()->get_rendering_thread()->AddRenderTask( newtask );
	return grc_succeeded;
}