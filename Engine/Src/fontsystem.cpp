#include "StdAfx.h"
#include "fontsystem.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

namespace mania
{
FT_Library library = NULL;

////////////////////
//fontface_freetype
////////////////////
class fontface_freetype: public fontface
{

public:
	FT_Face				m_face;
	unsigned char*		m_data;

	virtual ~fontface_freetype()
	{
		if ( m_data )
			delete[] m_data;
	}
};

////////////////////
//fontsystem_freetype
////////////////////
fontsystem_freetype::fontsystem_freetype( void )
{
}

fontsystem_freetype::~fontsystem_freetype( void )
{
}

gresult fontsystem_freetype::initialize()
{
	FT_Init_FreeType( &library );
	return grc_succeeded;
}

gresult fontsystem_freetype::uninitialize()
{
	FT_Done_FreeType( library );
	return grc_succeeded;
}

gresult fontsystem_freetype::create_face( sptr<fontface>& face, const string&  url )
{
	filesystem::file fontfile;
	fontface_freetype* newface = NULL;
	face = NULL;
	gresult gr = fontfile.open( url, filesystem::fileopen_binary | filesystem::fileopen_read );
	if ( succeeded( gr ) )
	{
		unsigned int len = fontfile.get_length();
		unsigned char* font_buffer = new unsigned char[len];
		fontfile.read( font_buffer, len, len );
		if ( !FT_New_Memory_Face( library, font_buffer, len, 0, &newface->m_face ) )
		{
			newface->m_data = font_buffer;
			face  = newface;
			return grc_succeeded;
		}
		else
			return grc_failed;
	}
	else
		return gr;
}


///////////////////////////////////
//quad_manager
///////////////////////////////////

void quad_manager::init( math::point store_size )
{
	m_store_size = store_size;
}

void quad_manager::addquad( const quad_identity& id, TCHAR data )
{
	//locate for store
}

void quad_manager::removequad( const quad_identity& id, TCHAR data )
{
}

void quad_manager::get_process_batch( const string&, quad_batch_group& qbg )
{
}

//提供id以及data，w，h，返回一个已安置的quad
gresult quad_manager::store_quad( const quad_identity& id, quad& in_quad, quad& out_quad )
{
	quad q = in_quad;
	storetype::iterator it;
	for ( it = m_stores.begin(); it != m_stores.end(); it++ )
		if ( store_quad( id, *it, q ) )
			return grc_succeeded;
	quad_store& newstore = add_store( id );
	if ( store_quad( id, newstore, q ) )
		return grc_succeeded;
	return grc_failed;
}
BOOL quad_manager::store_quad( const quad_identity& id, quad_store& store, quad& in_quad )
{
	if ( store.get_identity() == id )
		return FALSE;
	//check for freespace
	if ( store.quads.size() < store.m_max_count )
	{
		return TRUE;
	}
	return FALSE;
}

quad_store& quad_manager::add_store( const quad_identity& id )
{
	quad_store newstore;
	newstore.m_size = m_store_size;
	newstore.set_identity( id );
	m_stores.push_back( newstore );
	return m_stores.back();
}

//quad_store
void quad_store::set_identity( const quad_identity& id )
{
	m_id = id;
	//TODO: set max size;
}
}


