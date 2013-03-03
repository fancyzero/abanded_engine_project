#include "StdAfx.h"
#include "scene_renderer.h"

namespace mania
{
scene_renderer::scene_renderer( void )
{
}

scene_renderer::~scene_renderer( void )
{
}

void scene_renderer::render_all( sptr<node> root )
{
	std::vector<renderlist> renderlists;
	generate_renderlists( root, renderlists );
	render_renderlists( renderlists );
}

void scene_renderer::generate_renderlists( sptr<node> root, std::vector<renderlist>& renderlists )
{
	renderlist rl;
	renderlists.push_back( rl );
	node::iterator it( node::iterator::depth_first );
	for ( it = root; it; it++ )
		if ( renderable* ra = cast<renderable>( *it ) )
			renderlists[0].m_renderables.push_back( ra );
	//get_core()->get_log()->write( 1, 0xffffffff, L"found node: %s\n", name.c_str() );
}

void scene_renderer::render_renderlists( std::vector<renderlist>& renderlists )
{
	m_render_context.m_rendering_thread = get_core()->get_rendering_thread();
	m_render_context.m_viewinfo = m_viewinfo;
	for ( std::vector<renderlist>::iterator it = renderlists.begin(); it != renderlists.end(); it++ )
	{
		for ( std::vector<sptr<class renderable> >::iterator it2 = it->m_renderables.begin(); it2 != it->m_renderables.end(); it2++ )
		{
			//TODO : move view proj intto global param
			(*it2)->render( m_render_context );
		}
	}
}

gresult scene_renderer::set_viewtransform( const math::matrix44& t )
{
	m_viewinfo.set_viewmatrix(t);
	return grc_succeeded;
}

gresult scene_renderer::set_projtransform( const math::matrix44& t )
{
	m_viewinfo.set_projection(t);
	return grc_succeeded;
}

gresult scene_renderer::get_viewtransform( math::matrix44& t )
{
	t = m_viewinfo.get_viewmatrix();
	return grc_succeeded;
}

gresult scene_renderer::get_projtransform( math::matrix44& t )
{
	t = m_viewinfo.get_projection();
	return grc_succeeded;
}

}