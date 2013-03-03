#pragma once

namespace mania
{
	//struct defines
	struct raw_mesh_data
	{
		struct submesh
		{
			struct triangle_vert
			{
				triangle_vert()
					: vert_index( 0xffffffff ) 
				{
				};
				unsigned int id[16];
				mutable unsigned int vert_index;
			};

			struct material_record
			{
				unsigned int	start;
				unsigned int	length;
				unsigned int	matid;
			};

			typedef std::vector<std::pair< unsigned int, float > > bone_influnce;
			typedef std::map<string,unsigned int> srcindex_id_map;

			std::vector<math::vector3>		position_src;
			std::vector<math::vector3>		normal_src;
			std::vector<math::vector3>		tangent_src;
			std::vector<math::vector3>		binormal_src;
			std::vector<math::vector3>		uv_src;
			std::vector<triangle_vert>		verts;
			std::vector<material_record>	matrials;
			std::vector<bone_influnce>		bone_influences;
			unsigned int					position_vert_stride;
			unsigned int					vert_stride;
			srcindex_id_map					srcidx_id_map;
			
			bool	get_source_index( string sematic, unsigned int &id )
			{
				if ( srcidx_id_map.find(sematic) != srcidx_id_map.end() )
				{
					id = srcidx_id_map[sematic];
					return true;
				}
				else
					return false;
			}
		};

		struct bone
		{
			string				name;
			string				sid;
			int					index;//the index in the joint array
			math::matrix44		local_matrix;
			math::matrix44		offset_matrix;
			std::vector<int>	children;
		};

		math::matrix44				bind_shape_matrix;//each submesh or single,current is single, coz only one skeleton allowed.
		std::vector<submesh>		submeshes;
		std::vector<bone>			bones;
	};
}