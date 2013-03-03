#pragma once

namespace mania
{
class MANIA_ENGINE_API renderlist : public object
{
public:
	int	m_priority;
	std::vector<sptr<class renderable> >	m_renderables;
};

class MANIA_ENGINE_API scene_renderer : public object
{
public:
	scene_renderer( void );
	~scene_renderer( void );

	void render_all( sptr<node> root );
	void generate_renderlists( sptr<node> root, std::vector<renderlist>& renderlists );
	void render_renderlists( std::vector<renderlist>& renderlists );
	gresult set_viewtransform( const math::matrix44& t );
	gresult set_projtransform( const math::matrix44& t );
	gresult get_viewtransform( math::matrix44& t );
	gresult get_projtransform( math::matrix44& t );
protected:
	viewinfo				m_viewinfo;
	render_context			m_render_context;
	std::vector<renderlist>	m_renderlists[2];
};
}