#pragma once

namespace mania
{
class MANIA_ENGINE_API staticmesh : public mesh
{
public:
	staticmesh( void );
	~staticmesh( void );

	gresult load( const string& url );
private:
	gresult load_xml( const TCHAR* url );
protected:
	gresult build_render_data( struct raw_mesh_data* data );
};
}