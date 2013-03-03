#pragma once

namespace mania
{
	class viewinfo :public object
	{
		MANIA_DECLAR_CLASS(viewinfo)
	public:
		viewinfo(void);
		~viewinfo(void);

		void set_viewmatrix( const math::matrix44& mat )
		{
			m_viewmatrix = mat;
			update_view_proj();
		}
		void set_projection( const math::matrix44& mat )
		{
			m_projmatrix = mat;
			update_view_proj();
		}

		const math::matrix44& get_viewmatrix() const
		{
			return m_viewmatrix;
		}
		const math::matrix44& get_projection() const
		{
			return m_projmatrix;
		}

		const math::matrix44& get_view_proj()
		{
			return m_view_proj;
		}

	protected:
		void update_view_proj()
		{
			m_view_proj = m_viewmatrix * m_projmatrix;
		}

		math::matrix44	m_viewmatrix;
		math::matrix44	m_projmatrix;
		math::matrix44	m_view_proj;
	};
	struct render_context
	{
		rendering_thread*		m_rendering_thread;
		viewinfo				m_viewinfo;
	};
}