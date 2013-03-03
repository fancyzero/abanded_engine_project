#pragma once

namespace mania
{
	class MANIA_ENGINE_API skeletalmesh :
		public mesh
	{
	public:

		struct skeletal_vertex_raw
		{
			math::vector3	pos;
			math::vector2	uv;
			unsigned char	bone_id[4];
			float			bone_weight[4];
		};

		struct skeletal_vertex_posonly
		{
			math::vector3		pos;
			unsigned char		bone_id[4];
			float				bone_weight[4];
		};

		struct skeletal_vertex
		{
			math::vector2	uv;
		};

		struct bone
		{
			string			name;
			string			sid;
			int				index;
			math::matrix44	local_matrix;
			math::matrix44  offset_matrix;
			std::vector<int> children;
		};

	public:
		skeletalmesh(void);
		~skeletalmesh(void);

		virtual gresult load( const string& url );
		gresult load_from_bin( const TCHAR* filename );
		gresult load_from_xml( const TCHAR* filename );
		size_t	get_bone_count() const
		{
			return m_bones.size();
		}
		const bone& get_bone( int index ) const
		{
			return m_bones[index];
		}
		bone& get_bone( int index )
		{
			return m_bones[index];
		}		
		typedef std::vector<bone> bones;
		const bones& get_bones()// needed?
		{
			return m_bones;
		}
		const math::matrix44& get_bind_shape_matrix()
		{
			return m_bind_shape_matrix;
		}
	protected:
		gresult build_render_data( struct raw_mesh_data* data );
	protected:		
		bones			m_bones;
		math::matrix44	m_bind_shape_matrix;
	};
}