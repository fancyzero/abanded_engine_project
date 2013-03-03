#pragma once

namespace mania
{
template<class T>
class linknode
{
public:
	T*			data;
	linknode*	next;
};
template< T >
class linklist
{
public:
	typedef lnptr linknode<T>*;
	class iterator<lnptr>
	{
		iterator<lnptr>& operator ++()
		{
			current = m_current->next;
			return *this;
		}
		bool valid()
		{
			return ( current != NULL );
		}
		lnptr operator *()
		{
			return current;
		}
		lnptr		current;
	};
	lnkptr get_first() { return m_first; }
protected:
	lnkptr	first;
}

}