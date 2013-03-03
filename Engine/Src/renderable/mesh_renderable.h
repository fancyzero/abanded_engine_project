#pragma once

namespace mania
{
	class MANIA_ENGINE_API staticmesh_renderable : public renderable
	{
		MANIA_DECLAR_CLASS( staticmesh_renderable )
	public:

		void set_mesh( sptr<staticmesh> sm ){ m_staticmesh = sm; }
		sptr<staticmesh> get_mesh(){ return m_staticmesh; }
		virtual gresult render( render_context& render_context );
	protected:
		sptr<staticmesh>	m_staticmesh;
	};

	/// skeletalmesh_renderable
	class skeleton_posenode : public tnode
	{
	public:
		skeleton_posenode()
			:m_index(0)
		{
		}
		int			m_index;
	};

	class MANIA_ENGINE_API skeletalmesh_renderable : public renderable
	{
		MANIA_DECLAR_CLASS( skeletalmesh_renderable )
	public:
		skeletalmesh_renderable();
		~skeletalmesh_renderable();

		virtual gresult render( render_context& render_context );
		void set_mesh( sptr<skeletalmesh> sm );
		sptr<skeletalmesh> get_mesh(){ return m_skeletalmesh; }		
	protected:
		std::vector<sptr<skeleton_posenode> >		m_poses;
		math::matrix44*								m_current_matrix_array;
		sptr<skeletalmesh>							m_skeletalmesh;
	};
}