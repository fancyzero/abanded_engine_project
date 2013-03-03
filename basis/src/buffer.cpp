#include "StdAfx.h"
#include "buffer.h"

namespace mania
{
MANIA_IMPLEMENT_CLASS( buffer );

buffer::buffer()
		: m_size( 0 ), m_data( null )
{
	MANIA_CLASS_CONSTRUCTOR_INCLUDE( buffer );
};
buffer::buffer( void* data, unsigned int size )
		: m_data( data ), m_size( size )
{
	MANIA_CLASS_CONSTRUCTOR_INCLUDE( buffer );
}
buffer::~buffer( void )
{
	if ( m_data != NULL )
		delete m_data;
}
}