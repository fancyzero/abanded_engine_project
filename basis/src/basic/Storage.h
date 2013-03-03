#pragma once
#include <map>
namespace mania
{
template<class T>
class storage
{
public:
	storage( void )
	{
	}

	~storage( void )
	{
	}

	bool item_exist( const string& name )
	{
		return ( m_items.find( name ) != m_items.end() );
	}

	bool add_item( const string& name, T item )
	{
		if ( name.empty() )
			return false;
		_ASSERT( !item_exist( name ) );
		m_items[name] = item;
		return true;
	}

	bool remove_item( const string& name )
	{
		if ( name.empty() )
			return false;
		_ASSERT( item_exist( name ) );
		m_items.erase( name );
		return true;
	}

	T get_item( const string& name )
	{
		_ASSERT( item_exist( name ) );
		return m_items[name];
	}

	void clear() { m_items.clear(); }

	typedef	 typename std::map<string, typename T>::iterator iterator;

	iterator begin() { return m_items.begin(); }
	iterator end() { return m_items.end(); }
private:

	std::map<string, T>		m_items;

};
}