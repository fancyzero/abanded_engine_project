#include "StdAfx.h"
#include "xmlfile.h"
#include <assert.h>
#include "pugxml.h"

using namespace pug;

namespace mania
{
xmlfile::xmlfile( void )
		: m_parser( NULL )
{
}

xmlfile::~xmlfile( void )
{
	if ( m_parser )
		delete m_parser;
}

void walkthrough( pug::xml_node* node )
{
	for ( unsigned int i = 0;  i < node->children(); i++ )
	{
		walkthrough( &( node->child( i ) ) );
	}
}

DWORD xmlfile::open( const string& filename )
{
	if ( !m_parser )
		m_parser = new pug::xml_parser();
	if ( m_parser->parse_file( filename.c_str(), parse_trim_pcdata ) )
		return grc_succeeded;
	else
		return grc_failed;
}

xmlfile::node xmlfile::get_root()
{
	node n;
	if ( !m_parser )
		n = ( pug::t_xml_node_struct* )( NULL );
	else
		n = ( m_parser->document() );
	return n;
}

/////////////////
// xmlfile node
/////////////////

#define RETURN_IF_INVALID(x) if ( !m_node_struct ) return x;

xmlfile::node::node()
		: m_node_struct( NULL )
{
}
//xmlfile::node& operator xmlfile::node::=( xml_node* node )
//{
//	m_pugnode = node;
//	return *this;
//}
unsigned int xmlfile::node::get_child_count() const
{
	RETURN_IF_INVALID( NULL );
	return m_node_struct->children;
}
xmlfile::node xmlfile::node::get_child( unsigned int idx ) const
{
	xmlfile::node nd;
	nd = m_node_struct->getchild( idx );
	return nd;
}
const TCHAR* xmlfile::node::get_attribute( const string& attrname ) const
{
	RETURN_IF_INVALID( NULL );
	xml_node node( m_node_struct );
	pug::xml_attribute attr = node.getattribute( attrname.c_str() );
	return attr;
}
const TCHAR* xmlfile::node::get_value() const
{
	RETURN_IF_INVALID( NULL );
	for ( unsigned int i = 0; i < m_node_struct->children; i++ )
		if ( m_node_struct->getchild( i )->type_pcdata() )
			return m_node_struct->getchild( i )->value;
	return NULL;
}

const TCHAR* xmlfile::node::get_name() const
{
	RETURN_IF_INVALID( NULL );
	return m_node_struct->name;
}

xmlfile::node xmlfile::node::find_child( const string& name ) const
{
	if ( m_node_struct != NULL )
	for ( unsigned int i = 0; i < m_node_struct->childcount(); i++ )
		if ( name == m_node_struct->getchild( i )->name )
			return m_node_struct->getchild( i );
	return NULL;
}

xmlfile::node xmlfile::node::find_child( const string& name, const string& attr_name, const string& attr_value ) const
{
	RETURN_IF_INVALID( NULL );
	for ( unsigned int i = 0; i < m_node_struct->childcount(); i++ )
		if ( name == m_node_struct->getchild( i )->name )
			for ( unsigned int j = 0; j < m_node_struct->getchild( i )->attributes; j++ )
				if ( attr_name == m_node_struct->getchild( i )->attribute[j]->name )
					if ( attr_value == m_node_struct->getchild( i )->attribute[j]->value )
						return  m_node_struct->getchild( i );
	return NULL;
}

//node child_iterator
xmlfile::node::child_iteartor::child_iteartor( node& a_node, const string& a_name )
{
	nd = &a_node;
	name = a_name;
	current = 0xffffffff;
	//get first
	for ( unsigned int i = 0; i < nd->get_child_count(); i++ )
	{
		if ( nd->get_child( i ).get_name() == name )
		{
			current = i;
			return;
		}
	}
}
void xmlfile::node::child_iteartor::next()
{
	for ( unsigned int i = current + 1; i < nd->get_child_count(); i++ )
	{
		if ( nd->get_child( i ).get_name() == name )
		{
			current = i;
			return;
		}
	}
	current = 0xffffffff;
}
xmlfile::node xmlfile::node::child_iteartor::operator *()
{
	if ( current != 0xffffffff )
		return nd->get_child( current );
	else
		return NULL;
}
}