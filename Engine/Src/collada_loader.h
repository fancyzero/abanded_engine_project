#pragma once

namespace mania
{
	namespace collada_data
	{
		struct mesh;
		struct input;
		struct source;
		struct xml_data;
		struct scene_node;
		struct skin_entity;
	}
	///load collada .dae file into raw data
	class collada_loader
	{
	public:
		collada_loader(void);
		~collada_loader(void);
		gresult build_raw_data( const TCHAR* filename );
		raw_mesh_data& get_raw_data(){ return raw_data; }
	protected:
		gresult build_submesh( collada_data::xml_data* cd, const struct collada_data::mesh* collada_mesh, raw_mesh_data::submesh& sm );
		gresult read_input( const struct collada_data::input* inp, const struct collada_data::mesh* collada_mesh, raw_mesh_data::submesh& sm );
		gresult read_input( const struct collada_data::input* inp, const std::vector<sptr<collada_data::source> > sources, void* host );
		collada_data::scene_node* find_root_bone( collada_data::scene_node* scene_root );
		gresult	read_skeleton( collada_data::skin_entity* skin, collada_data::scene_node* root, int parent_index );
		raw_mesh_data	raw_data;
	};
}