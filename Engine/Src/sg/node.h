#pragma once

namespace mania
{
typedef sptr<class node> node_ptr;
class MANIA_ENGINE_API node : public object
{
	MANIA_DECLAR_CLASS( node )
public:
	typedef std::list<node_ptr> nodelist_type;
	class iterator
	{
	public:
		struct  recursion_stack_node
		{
			node_ptr					node;
			nodelist_type::iterator		subnode_it;
		};
		enum iterator_method
		{
			depth_first,
			breadth_first,
		};
		iterator( iterator_method method_ )
				: method( method_ ), initialized( false ), root( null ), current( null )
		{}

		operator bool();
		iterator& operator =( node* a );
		node*	operator*();
		void reset( node* a );
		void advance();
		iterator& operator ++();
		iterator operator ++( int );
		void advance_df();
		void advance_bf();
	private:
		bool								initialized;
		node*								root;
		node*								current;
		iterator_method						method;
		std::stack<recursion_stack_node>	recursion_stack;
	};
	node( void );
	virtual ~node( void );
	node& operator =(const node& a)
	{
		m_subnodes = a.m_subnodes;
		m_parent = m_parent;
		return *this;
	}
	nodelist_type& get_subnodes()
	{
		return m_subnodes;
	}
	node_ptr get_parent()
	{
		return m_parent;
	}
	void			attach( node_ptr subnode );
	void			detach( node_ptr subnode );
	void			detach_children();
	std::size_t		get_child_count();
	node*			get_child( std::size_t index );
	virtual	void	update( float lag );
protected:
	void	set_parent( node_ptr parent );
	void	add_subnode( node_ptr subnode );
	void	remove_subnode( node_ptr subnode );
protected:

	nodelist_type	m_subnodes;
	node_ptr		m_parent;
};
}