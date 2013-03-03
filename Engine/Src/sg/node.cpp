#include "StdAfx.h"
#include "node.h"

namespace mania
{
///////////////////////////////////////////////////////
//node::iterator
///////////////////////////////////////////////////////
node::iterator& node::iterator::operator =( node* a )
{
	reset( a );
	return *this;
}

void node::iterator::reset( node* a )
{
	root = a;
	while ( !recursion_stack.empty() )
		recursion_stack.pop();
	recursion_stack_node rsn;
	rsn.node = root;
	rsn.subnode_it = rsn.node->m_subnodes.begin();
	recursion_stack.push( rsn );
	advance();
}

void node::iterator::advance()
{
	switch ( method )
	{
	case depth_first:
		advance_df();
		break;
	case breadth_first:
		advance_bf();
		break;
	}
}

node::iterator& node::iterator::operator ++()
{
	advance();
	return *this;
}

node::iterator node::iterator::operator ++( int )
{
	node::iterator tmp = *this;
	advance();
	return tmp;
}

void node::iterator::advance_df()
{
	while ( true )
	{
		if ( recursion_stack.empty() )
		{
			current = NULL;
			return;
		}
		recursion_stack_node& top = recursion_stack.top();
		if ( top.subnode_it != top.node->m_subnodes.end() )
		{
			recursion_stack_node newtop;
			newtop.node = ( *top.subnode_it );
			newtop.subnode_it = newtop.node->m_subnodes.begin();
			recursion_stack.push( newtop );
		}
		else
		{
			current = top.node;
			recursion_stack.pop();
			if ( !recursion_stack.empty() )
				recursion_stack.top().subnode_it++;
			break;
		}
	}
}

void node::iterator::advance_bf()
{
	//TODO: implement it
}
node::iterator::operator bool()
{
	return current != null;
}
node* node::iterator::operator*()
{
	return current;
}
///////////////////////////////////////////////////////
//node
///////////////////////////////////////////////////////
MANIA_IMPLEMENT_CLASS( node );
node::node( void )
{
	MANIA_CLASS_CONSTRUCTOR_INCLUDE( node );
}

node::~node( void )
{
}
void node::attach( node_ptr subnode )
{
	subnode->set_parent( this );
	add_subnode( subnode );
}
void node::detach( node_ptr subnode )
{
	_ASSERT( subnode->get_parent() == this );
	remove_subnode( subnode );
	subnode->set_parent( NULL );
}
void node::detach_children()
{
	nodelist_type::iterator it = m_subnodes.begin();
	for ( ; it != m_subnodes.end(); it = m_subnodes.begin()  )
		detach(*it);
}
std::size_t node::get_child_count()
{
	return m_subnodes.size();
}
node* node::get_child( std::size_t index )
{
	if ( index >= get_child_count() )
		return NULL;
	nodelist_type::iterator it;
	std::size_t n = 0;
	it = m_subnodes.begin();
	while( n < index )
		it++;
	return *it;
}
void node::update( float lag )
{
	for ( nodelist_type::iterator it = m_subnodes.begin(); it != m_subnodes.end(); ++it )
	{		
		(*it)->update( lag );
	}
}
////////////////
void node::set_parent( node_ptr parent )
{
	m_parent = parent;
}
void node::add_subnode( node_ptr subnode )
{
	m_subnodes.push_back( subnode );
}
void node::remove_subnode( node_ptr subnode )
{
	nodelist_type::iterator it;
	for ( it = m_subnodes.begin(); it != m_subnodes.end(); ++it )
	{
		if ( *it == subnode )
		{
			m_subnodes.erase( it );
			break;
		}
	}
}
}