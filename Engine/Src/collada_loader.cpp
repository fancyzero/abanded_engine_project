#include "StdAfx.h"
#include "mesh_data_defines.h"
#include "collada_loader.h"

namespace mania
{
	typedef xmlfile::node xml_node_type;

	float wtof_fast( const TCHAR** refref_s)
	{
		const TCHAR *&s = *refref_s;
		double sig = 1.0;
		while(*s)
		{
			if (*s == L'-')
			{
				sig = -1.0;
				s++;
			}
			if ( *s >=L'0' && *s <=L'9' )
				break;
			if ( *s==L'.' )
				break;
			s++;
		}
		double float_part = 0;
		double frac_part = 0;

		bool is_frac_part = false;
		int	frac_n = 10;
		bool contact = false;
		while (*s)
		{
			if ( *s >=L'0' && *s <=L'9' )
			{
				contact = true;
				if ( is_frac_part )
				{
					frac_part += (*s - L'0') * (1.0/frac_n);
					frac_n*=10;
				}
				else
					float_part = float_part*10 + *s - L'0';
			}
			else
				if ( *s==L'.' )
				{
					contact = true;
					is_frac_part = true;
				}
				else
					if (contact)
						break;
			s++;
		}	
		return (float)((float_part + frac_part)*sig);
	};


	string wtostring( const TCHAR** s )
	{
		string ret;
		while( **s && isspace(**s) )
			(*s)++;
		while( **s && !isspace(**s) )
		{
			ret += **s;
			(*s)++;
		}
		return ret;
	}
	
	void convert( const TCHAR** s, float& t )
	{
		t = wtof_fast(s);
	}

	void convert( const TCHAR** s, int& t )
	{
		t = (int)wtof_fast(s);
	}

	void convert( const TCHAR** s, unsigned int& t )
	{
		t = (int)wtof_fast(s);
	}

	void convert( const TCHAR** s, string& t )
	{
		t = wtostring(s);
	}

	template< class T >
	void read_array_string( const TCHAR* s, std::vector<T>& vector_array )
	{
		enum convert_state
		{
			read_component,
			search_begin_of_next_1,
			search_begin_of_next_2,
			push_vector
		};
		vector_array.clear();
		convert_state state = read_component;
		int cur_comp = 0;
		if ( !s )
			return;
		while ( *s )
		{
			//switch ( state )
			//{
			//case read_component:
			//	{
					T t;
					convert( &s, t );
					vector_array.push_back( t );
					//state = search_begin_of_next_1;
				//}
				//break;
			//case search_begin_of_next_1:
			//	if ( isspace( *s ) )
			//		s++;
			//	else
			//		state = search_begin_of_next_2;
			//	break;
			//case search_begin_of_next_2:
			//	if ( !isspace( *s ) )
			//		s++;
			//	else
			//		state = read_component;
			//	break;
			//}
		}
	}

	

	namespace collada_data
	{
		struct _base : public ref_counted
		{
			
		};
		template<class Factory, class T >
		gresult dispatch_to_subnodes( std::vector<sptr<T> >& ar, xml_node_type& root )
		{
			for (  unsigned int i = 0; i < root.get_child_count(); i++ )
			{
				xml_node_type& child_node = root.get_child(i);
				sptr<T> t = Factory::create_by_name( child_node.get_name() );
				if ( failed(t->parse(child_node)) )
					return grc_failed;
				ar.push_back(t);
			}
			return grc_succeeded;
		}
		template<class T >
		gresult dispatch_to_subnodes( std::vector<sptr<T> >& ar, xml_node_type& root, TCHAR* nodename )
		{
			for ( xml_node_type::child_iteartor it( root, nodename ); (*it) != NULL; it.next() )
			{
				sptr<T> t = new T();
				if ( failed(t->parse(*it)) )
					return grc_failed;
				ar.push_back(t);
			}
			return grc_succeeded;
		}
		struct accessor_ :public _base
		{
			accessor_():count(0),stride(0){}
			mania::string	source_id;
			unsigned int	count;
			unsigned int	stride;
			std::vector<mania::string> params;
			gresult parse( xml_node_type& root )
			{
				source_id = root.get_attribute(L"source" );
				count = ::_wtoi( root.get_attribute(L"count") );
				stride = ::_wtoi( root.get_attribute(L"stride") );
				for ( xml_node_type::child_iteartor it( root, L"param" ); (*it) != NULL; it.next() )
				{
					params.push_back((*it).get_attribute(L"name"));
				}
				return grc_succeeded;
			}
		};

		struct _base_array :public _base
		{
			_base_array():count(0){};
			mania::string	id;
			unsigned int	count;
			virtual gresult parse( xml_node_type& root ) = 0;
		};

		template<typename T>
		struct array_ :public _base_array
		{
			std::vector<T>	data;
			virtual gresult parse( xml_node_type& root )
			{
				id = root.get_attribute(L"id" );
				count = ::_wtoi( root.get_attribute(L"count") );
				read_array_string( root.get_value(), data );
				return grc_failed;
			}
		};

		struct source:public _base
		{	
			mania::string		id;
			accessor_			accessor;
			sptr<_base_array>	data_array;			
			virtual gresult parse( xml_node_type& root )
			{
				id = root.get_attribute(L"id" );
				gresult gr1 = grc_failed;
				gresult gr2 = grc_failed;
				xml_node_type tmpnode;
				if ( root.find_child(L"float_array").valid() )
				{
					data_array = new array_<float>();
					gr1 = data_array->parse(root.find_child(L"float_array"));
				}
				else
				if ( root.find_child(L"Name_array").valid() )
				{
					data_array = new array_<string>();
					gr1 = data_array->parse(root.find_child(L"Name_array"));
				}
				else
				if ( root.find_child(L"IDREF_array").valid() )
				{
					data_array = new array_<string>();
					gr1 = data_array->parse(root.find_child(L"IDREF_array"));
				}
				tmpnode = root.find_child(L"technique_common").find_child(L"accessor");
				if ( tmpnode.valid() )
					gr2 = accessor.parse(tmpnode);

				if ( succeeded(gr1) && succeeded(gr2) )
					return grc_succeeded;
				else
					return grc_failed;
			}
			unsigned int get_count()
			{
				return data_array->count;
			}
			const float& get_float( unsigned int index ) const
			{
				return ((array_<float>*)data_array)->data[index];
			}
			const string& get_string( unsigned int index ) const
			{
				return ((array_<string>*)data_array)->data[index];
			}
			unsigned int find_string( const TCHAR* str )
			{
				for( unsigned int i = 0; i < data_array->count; i++ )
					if ( ((array_<string>*)data_array)->data[i] == str )
						return i;
				return 0xffffffff;
			}
		};
		struct input:public _base
		{
			mania::string semantic;
			mania::string source_id;
			unsigned int  offset;
			unsigned int  set;
			input():set(1),offset(0){}
			gresult parse( xml_node_type& root )
			{
				semantic = root.get_attribute( L"semantic" );
				source_id = root.get_attribute( L"source" );
				offset = _wtoi(root.get_attribute( L"offset" ));
				set = _wtoi(root.get_attribute( L"set" ));
				if ( set == 0 )
					set = 1;
				return grc_succeeded;
			}
			unsigned int get_stride() const
			{
				return set;
			}
		};
		struct triangles:public _base
		{
			triangles():count(0),index_stride(0){}
			std::vector<sptr<input> >	inputs;
			std::vector<unsigned int> indecies;
			mania::string		material;
			unsigned int		count;
			unsigned int		index_stride;
			gresult parse( xml_node_type& root )
			{
				gresult gr = dispatch_to_subnodes( inputs, root, L"input" );	
				return_if_failed(gr);
				read_array_string<unsigned int>( root.find_child(L"p").get_value(), indecies );
				material = root.get_attribute(L"material");
				count = _wtoi(root.get_attribute(L"count"));
				index_stride = 0;
				foreach( std::vector< sptr<input> >, inp, inputs )
				{
					index_stride += (*inp)->get_stride();
				}
				return grc_succeeded;
			}
		};
		struct vertices:public _base
		{
			mania::string		source_id;
		};
		struct mesh:public _base
		{
			std::vector< sptr<source> >			sources;
			vertices							verts;
			std::vector< sptr<triangles> >		triangles_groups;

			gresult parse( xml_node_type& root )
			{
				gresult gr = dispatch_to_subnodes( sources, root, L"source" );
				return_if_failed(gr);
				verts.source_id = root.find_child( L"vertices" ).find_child(L"input").get_attribute(L"source");
				gr = dispatch_to_subnodes( triangles_groups, root, L"triangles" );
				return_if_failed(gr);
				return grc_succeeded;
			}
		};
		struct geometry:public _base
		{
			mania::string id;
			mania::string name;
			std::vector< sptr<mesh> >	meshes;
			gresult parse( xml_node_type& root )
			{
				id = root.get_attribute(L"id" );
				name = root.get_attribute(L"name" );
				gresult gr = dispatch_to_subnodes( meshes, root, L"mesh" );
				return gr;
			}
		};
		struct library_geometries:public _base
		{
			std::vector< sptr<geometry> > geometries;
			gresult parse( xml_node_type& root )
			{
				return dispatch_to_subnodes( geometries, root, L"geometry" );
			}
		};
		struct material:public _base
		{
			string id;
			string name;
			gresult parse( xml_node_type& root )
			{
				id = root.get_attribute(L"id");
				name = root.get_attribute(L"name");
				return grc_succeeded;
			}
		};
		struct library_material:public _base
		{
			std::vector< sptr<material> > materials;
			gresult parse( xml_node_type& root )
			{
				return dispatch_to_subnodes( materials, root, L"material" );
			}
		};
		struct controller_entity: public _base
		{
			string id;
			string name;
			virtual gresult parse( xml_node_type& root )
			{
				id = root.get_attribute(L"id");
				name = root.get_name();
				return grc_succeeded;
			}
		};
		struct skin_joints: public _base
		{
			std::vector<sptr<input> >	inputs;
			gresult parse( xml_node_type& root )
			{
				gresult gr = dispatch_to_subnodes( inputs, root, L"input" );	
				return grc_succeeded;
			}
		};
		struct skin_vertex_weights: public _base
		{
			gresult parse( xml_node_type& root )
			{
				read_array_string<unsigned int>( root.find_child(L"vcount").get_value(), vcount );
				read_array_string<unsigned int>( root.find_child(L"v").get_value(), v );
				gresult gr = dispatch_to_subnodes( inputs, root, L"input" );	
				return grc_succeeded;
			}
			std::vector<sptr<input> >	inputs;
			std::vector<unsigned int>	vcount;
			std::vector<unsigned int>	v;
		};
		struct skin_entity:public controller_entity
		{
			math::matrix44				bind_shape_matrix;
			std::vector<sptr<source> >	sources;
			skin_vertex_weights			weights;
			skin_joints					joints;
			virtual gresult parse( xml_node_type& root )
			{
				return_if_failed(controller_entity::parse( root ));
				if ( root.find_child(L"bind_shape_matrix").valid() )
				{
					std::vector<float> floats;
					read_array_string( root.find_child( L"bind_shape_matrix" ).get_value(), floats );
					if ( floats.size() < 16 )
						return grc_failed;
					for ( int i = 0; i < 4; i++ )
						for ( int j = 0; j < 4; j++ )
							bind_shape_matrix.m[i][j] = floats[i+j*4];					
				}
				return_if_failed( dispatch_to_subnodes( sources, root, L"source" ) );
				return_if_failed( weights.parse( root.find_child( L"vertex_weights" ) ) );
				return_if_failed( joints.parse( root.find_child( L"joints" ) ) );
				
				return grc_succeeded;
			}
		};
		class controller_entity_factory
		{
		public:
			static controller_entity* create_by_name( const string& name )
			{
				if ( name == L"skin" )
					return new skin_entity();
				return NULL;
			}
		};

		struct controller:public _base
		{
			string id;
			std::vector<sptr<controller_entity> > controller_entities;
			virtual gresult parse( xml_node_type& root )
			{
				id = root.get_attribute(L"id");
				dispatch_to_subnodes<controller_entity_factory>( controller_entities, root );
				return grc_succeeded;
			}
		};
		struct library_controller:public _base
		{
			std::vector< sptr<controller> > controllers;
			gresult parse( xml_node_type& root )
			{
				return dispatch_to_subnodes( controllers, root, L"controller" );
			}
		};
		struct scene_node: public _base
		{
			string id;
			string name;
			string sid;
			string type;
			math::matrix44 local_matrix;
			std::vector<scene_node> children;
			gresult parse( xml_node_type& root )
			{
				id = root.get_attribute( L"id" );
				name = root.get_attribute( L"name" );
				sid = root.get_attribute( L"sid" );
				type = root.get_attribute( L"type" );
				std::vector<float> floats;
				read_array_string( root.find_child(L"matrix").get_value(), floats );
				if ( floats.size() >= 16 )
				{
					for ( int m = 0; m < 4; m++ )
						for ( int n = 0; n < 4; n++ )
							local_matrix.m[m][n] = floats[m+n*4];	
				}
				else
				{
					local_matrix.set_identity();
				}
				for ( xml_node_type::child_iteartor it( root, L"node" ); (*it).valid(); it.next() )
				{
					scene_node newnode;
					newnode.parse( *it );
					children.push_back( newnode );
				}
				return grc_succeeded;
			}
		};

		struct library_visual_scenee:public _base
		{
			scene_node scen_root;
			gresult parse( xml_node_type& root )
			{
				return scen_root.parse( root.find_child( L"visual_scene" ) );
			}
			~library_visual_scenee()
			{
				GetTickCount();
			}
		};

		struct xml_data
		{
			library_geometries		lib_geo;
			library_material		lib_mat;
			library_controller		lib_controller;
			library_visual_scenee	lib_visual_scene;
			gresult parse( xml_node_type& root )
			{
				xml_node_type lib_node = root.find_child( L"COLLADA" ).find_child( _T( "library_geometries" ) );
				if ( lib_node.valid() )
					return_if_failed( lib_geo.parse(lib_node) );

				lib_node = root.find_child( L"COLLADA" ).find_child( _T( "library_materials" ) );
				if ( lib_node.valid() )
					return_if_failed( lib_mat.parse(lib_node) );

				lib_node = root.find_child( L"COLLADA" ).find_child( _T( "library_controllers" ) );
				if ( lib_node.valid() )
					return_if_failed( lib_controller.parse(lib_node) );
				
				lib_node = root.find_child( L"COLLADA" ).find_child( _T( "library_visual_scenes" ) );
				if ( lib_node.valid() )
					return_if_failed( lib_visual_scene.parse(lib_node) );

				return grc_succeeded;				
			}
		};

		sptr<collada_data::input> find_input( const std::vector< sptr<input> >& inputs, const TCHAR* semantic )
		{
			foreachc( std::vector< sptr<input> >, inp, inputs )
				if ( (*inp)->semantic == semantic )
					return (*inp);
			return NULL;
		}

		sptr<source> find_source( const std::vector< sptr<source> >& sources, const TCHAR* id )
		{
			foreachc( std::vector< sptr<source> >, src, sources )
				if ( string(L"#") + (*src)->id == id )
					return (*src);
			return NULL;
		}

		sptr<source> find_input_source( const std::vector< sptr<input> >& inputs, const std::vector< sptr<source> >& sources, const TCHAR* inp_semantic  )
		{
			sptr<collada_data::input> inp = find_input( inputs, inp_semantic );
			if ( inp )
				return find_source( sources, inp->source_id.c_str() );
			return NULL;
		}
	}

	collada_loader::collada_loader(void)
	{
	}

	collada_loader::~collada_loader(void)
	{
	}

	//build raw mesh data from the first mesh in the collada gemoety library
	gresult collada_loader::build_raw_data( const TCHAR* filename )
	{
		xmlfile xf;
		gresult gr;
		gr = xf.open( filename );
		if ( failed ( gr ) )
		{
			get_log()->write( 1, 0xff0000, ::mania::get_string( L"COLLADA_PARSXML_FAILED" ).c_str() );
			return gr;
		}
		xml_node_type n = xf.get_root();
		collada_data::xml_data cd;	
		gr = cd.parse(n);
		if ( failed(gr) )
			return gr;

		raw_data.submeshes.resize( cd.lib_geo.geometries.size() );
		size_t i = 0;
		foreach( std::vector< sptr<collada_data::geometry> >, geo, cd.lib_geo.geometries )
			foreach( std::vector< sptr<collada_data::mesh> >, m, (*geo)->meshes )
			{
				gr = build_submesh( &cd, (collada_data::mesh*)(*m), raw_data.submeshes[i] );
				if ( failed(gr) )
					return gr;
				i++;
				break;//assume only one mesh per geometry
			}		
		return grc_succeeded;
	}

	//there is always only one skeletal in the scene, so deepth-first search is ok.
	collada_data::scene_node* collada_loader::find_root_bone( collada_data::scene_node* scene_root )
	{
		collada_data::scene_node* ret;
		if ( scene_root->type == L"JOINT" )
			return scene_root;
		for ( unsigned int i = 0; i < scene_root->children.size(); i++ )
		{
			ret = find_root_bone( &scene_root->children[i] );
			if ( ret != NULL )
				return ret;
		}
		return NULL;
	}

	gresult read_joints( collada_data::skin_entity* skine,
		std::vector<raw_mesh_data::bone>& bones )
	{
		if ( skine == NULL )
			return grc_failed;
		sptr<collada_data::input> inp_inv_matrix = collada_data::find_input( skine->joints.inputs, L"INV_BIND_MATRIX");
		sptr<collada_data::input> inp_joints = collada_data::find_input( skine->joints.inputs, L"JOINT");
		if ( inp_inv_matrix != NULL )
		{
			collada_data::source* src_inv_matrix = collada_data::find_source( skine->sources, inp_inv_matrix->source_id.c_str() );
			collada_data::source* src_joints = collada_data::find_source( skine->sources, inp_joints->source_id.c_str() );
			if ( src_inv_matrix == NULL || src_joints == NULL )
				return grc_failed;	
			if ( src_inv_matrix->accessor.count < src_joints->accessor.count )
				::mania::get_log()->write( 1, 0xff0000, L"error: inv_matrix and joints count not match." );
			for ( unsigned int i = 0; i < src_inv_matrix->accessor.count; i++ )
			{
				for ( unsigned int k = 0; k < bones.size(); k++ )
				{
					if ( bones[k].sid == src_joints->get_string(i) )
					{
						//for ( unsigned int j = 0; j < 16; j++ )
						//	bones[k].offset_matrix.m[j%4][j/4] = src_inv_matrix->get_float(j+i*16);

						for ( int m = 0; m < 4; m++ )
							for ( int n = 0; n < 4; n++ )
								bones[k].offset_matrix.m[m][n] = src_inv_matrix->get_float(m+n*4+i*16);	
						//bones[k].offset_matrix.inverse();
						break;
					}					
				}
			}
		}
		return grc_failed;
	}

	//fill bones and setup skeleton
	gresult	collada_loader::read_skeleton( collada_data::skin_entity* skin, collada_data::scene_node* root, int parent_index )
	{
		if ( skin == NULL )
			return grc_failed;
		int cur_index = 0;
		if ( root == NULL )
			return grc_failed;
		if ( root->type == L"JOINT" )
		{
			cur_index = (int)raw_data.bones.size();	
			raw_mesh_data::bone bone;
			bone.name = root->name;
			bone.sid = root->sid;
			bone.local_matrix = root->local_matrix;
			//find the index in the joints array of this bone
			bone.index = -1;
			sptr<collada_data::source> src_joints = find_input_source( skin->joints.inputs, skin->sources, L"JOINT" );
			if ( src_joints )
			{
				unsigned int ret = src_joints->find_string(bone.sid.c_str());
				if ( ret != 0xffffffff )
					bone.index = ret;
				else
					mania::get_log()->write( 0, 0xff0000, L"JOINT %s not found.", bone.sid.c_str() );
			}
			//---------
			if ( parent_index != -1 )
				raw_data.bones[parent_index].children.push_back( cur_index );
			raw_data.bones.push_back( bone );
			for ( unsigned int i = 0; i < root->children.size(); i++ )
				read_skeleton( skin, &root->children[i], cur_index );
		}
		return grc_succeeded;
	}
	//utility function for copy data
	gresult collada_loader::build_submesh( collada_data::xml_data* cd, const struct collada_data::mesh* collada_mesh, mania::raw_mesh_data::submesh& sm )
	{	
		const collada_data::source* src = NULL;
		
		//TODO : read_vert_inputs() and read_triangle_inputs() will be better.
		if ( find_source( collada_mesh->sources, collada_mesh->verts.source_id.c_str() ) )	
		{
			src = (collada_data::source*)(find_source( collada_mesh->sources, collada_mesh->verts.source_id.c_str() ).GetRealPointer());
			sm.position_vert_stride = src->accessor.stride;
			for ( unsigned int n = 0; n < src->data_array->count; n += 3 )
				sm.position_src.push_back( math::vector3( src->get_float(n), src->get_float(n+1), src->get_float(n+2) ) );
		}

		sm.vert_stride = 0;
		if ( !collada_mesh->triangles_groups.empty() )
		{
			//Assume all <triangles>'s inputs are same, so just use first <triangles>'s sources to fill the raw_mesh_data.
			foreachc( std::vector< sptr<collada_data::input> >, inp, collada_mesh->triangles_groups[0]->inputs )
			{
				read_input( (*inp), collada_mesh, sm );
				sm.vert_stride += (*inp)->get_stride();
			}
		}
		int matid = 0;
		foreachc( std::vector< sptr<collada_data::triangles> >, tris_groups, collada_mesh->triangles_groups )
		{
			raw_mesh_data::submesh::material_record mr;
			mr.start = (unsigned int)sm.verts.size();
			for( unsigned int i = 0; i < (*tris_groups)->count*3; i++ )
			{
				raw_mesh_data::submesh::triangle_vert tv;
				unsigned int offset = i*(*tris_groups)->index_stride;
				foreachc( std::vector< sptr<collada_data::input> >, inp, (*tris_groups)->inputs )
				{
					for ( unsigned int s = 0; s < (*inp)->set; s++ )
						tv.id[(*inp)->offset+s] = (*tris_groups)->indecies[ offset + (*inp)->offset+s];
				}
				sm.verts.push_back(tv);
			}
			mr.matid = matid;
			mr.length = (unsigned int)sm.verts.size() - mr.start;
			sm.matrials.push_back(mr);
			matid++;
		}
		//read the only one skeletal infomation
		collada_data::scene_node* root_bone = find_root_bone( &cd->lib_visual_scene.scen_root );
		
		//process controllers
		foreach( std::vector< sptr<collada_data::controller> >, ctrl, cd->lib_controller.controllers )
			foreach( std::vector<sptr<collada_data::controller_entity> >, ctrl_e, (*ctrl)->controller_entities )
				if ( (*ctrl_e)->name == L"skin" )
				{
					collada_data::skin_entity* skine = *ctrl_e;				
					this->raw_data.bind_shape_matrix = skine->bind_shape_matrix;
					read_skeleton( skine, root_bone, -1 );
					read_joints( skine, raw_data.bones );					
					sptr<collada_data::input> weights_inp = collada_data::find_input( skine->weights.inputs, L"WEIGHT" );
					if ( weights_inp )
					{
						sptr<collada_data::source> weight_src = collada_data::find_source( skine->sources, weights_inp->source_id.c_str() );
						if ( weight_src )
						{
							int idx = 0;
							for ( int i = 0; i< skine->weights.vcount.size(); i++ )
							{
								std::vector<std::pair<unsigned int, float> > v;
								for ( unsigned int j = 0; j < skine->weights.vcount[i]; j++ )
								{		
									std::pair<unsigned int, float> p;
									p.first = skine->weights.v[idx++];

									p.second = weight_src->get_float(skine->weights.v[idx++]);
									v.push_back(p);
								}
								sm.bone_influences.push_back(v);
							}
						}
					}
				}
		return grc_succeeded;
	}

	gresult collada_loader::read_input( const struct collada_data::input* inp, const struct collada_data::mesh* collada_mesh, raw_mesh_data::submesh& sm )
	{
		const collada_data::source* src = NULL;
		string srcid = inp->source_id;
		src = find_source(collada_mesh->sources, inp->source_id.c_str());
		if ( src == NULL )
			return grc_succeeded;
		//for vector3
		std::vector<vector3>* v3array = NULL;
		if (inp->semantic == L"NORMAL")
			v3array = &sm.normal_src;
		if (inp->semantic == L"TEXCOORD")//TODO : take the "set" in count
			v3array = &sm.uv_src;
		if ( v3array )
		{			
			for ( unsigned int n = 0; n < src->data_array->count; n += src->accessor.stride )
				v3array->push_back( math::vector3( src->get_float(n), src->get_float(n+1), src->get_float(n+2) ) );		
		}
		return grc_succeeded;
	}
}