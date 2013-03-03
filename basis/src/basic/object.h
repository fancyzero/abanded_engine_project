#pragma once

namespace mania
{
class MANIA_BASIS_API identifier
{
public:
//		static trait t;
	static critical_section cs;
	static std::map<mania::string, int> counter;
	std::map<mania::string, int>::iterator it;
	int 	serial_number;
	identifier()
	{
		serial_number = -1;
		cs.lock();
		it = counter.end();
		cs.unlock();
	}
	identifier& operator = ( const mania::string& str )
	{
		cs.lock();
		serial_number = counter[str]++;
		it = counter.find( str );
		cs.unlock();
		return *this;
	}
	BOOL empty()
	{
		cs.lock();
		BOOL b = it == counter.end();
		cs.unlock();
		return b;
	}

	const mania::string& get_string_part()
	{
		if ( empty() )
			return get_string( _T( "ID_NOT_INITIALIZED" ) );
		else
			return ( *it ).first;
	}

	int get_serial()
	{
		return serial_number;
	}
};

class MANIA_BASIS_API object : public ref_counted
{
	MANIA_DECLAR_CLASS( object )
public:
	object();
	virtual ~object();
	const mania::string& get_name() { return m_id.get_string_part(); }
	void set_name( const mania::string& name ) { m_id = name; }
	bool is( const classinfo* aclassinfo );
	//gc
	void set_outer( object* in_outer ) { m_outer = in_outer; }
	object* get_outer() { return m_outer; }
protected:
	bool match_classinfo( const classinfo* aclassinfo );

	classinfo*	m_classinfo;
	identifier	m_id;
	object*		m_outer;
	DWORD		m_tag;
};
MANIA_DEFINE_PTR( object );

template<class T>
T* construct_object( const TCHAR* name, object* in_outer )
{
	T* newT = ( T* )( T::get_classinfo()->construct() );
	newT->set_outer( in_outer );
	if ( name == NULL || name[0] == 0 )
		newT->set_name( T::get_classinfo()->name );
	else
		newT->set_name( name );
	get_log()->write( 1, 0xffffffff, L"construct object, class<%s>, name\"%s\"\r\n", T::get_classinfo()->name.c_str(), name );
	return newT;
}

template< class T>
T* cast( object* obj )
{
	if ( obj->is( T::get_classinfo() ) )
		return (T*)( obj );
	else
		return null;
}
}