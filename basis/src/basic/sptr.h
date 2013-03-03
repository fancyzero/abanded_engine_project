#pragma once

namespace mania
{
template<class T>
class sptr
{
	friend class sptr;
public:

	sptr( T* p = NULL )
	{
		m_pRefObj = p;
		if ( m_pRefObj != NULL )
			m_pRefObj->addref();
	}

	~sptr( void )
	{
		if ( m_pRefObj != NULL )
			m_pRefObj->release();
	}

	sptr( const sptr& InCopy )
			: m_pRefObj( NULL )
	{
		Reset( InCopy.m_pRefObj );
	}

	template<class Y>
	sptr( const sptr<Y>& InCopy )
			: m_pRefObj( NULL )
	{
		Reset( InCopy.m_pRefObj );
	}

	T*	GetRealPointer() const
	{
		return m_pRefObj;
	}

	template<class Y>
	void Reset( Y* p )
	{
		if ( m_pRefObj != NULL )
			m_pRefObj->release();
		m_pRefObj = ( T* )p;
		if ( m_pRefObj != NULL )
			m_pRefObj->addref();
	}

	void Reset( T* p )
	{
		if ( m_pRefObj != NULL )
			m_pRefObj->release();
		m_pRefObj = p;
		if ( m_pRefObj != NULL )
			m_pRefObj->addref();
	}

	sptr& operator = ( sptr& v )
	{
		if ( m_pRefObj != v.m_pRefObj )
		{
			Reset( v.m_pRefObj );
		}
		return *this;
	}

	template<class Tother>
	sptr& operator = ( sptr<Tother>& v )
	{
		if ( m_pRefObj != v.m_pRefObj )
		{
			Reset( v.m_pRefObj );
		}
		return *this;
	}

	sptr& operator = ( T* p )
	{
		if ( m_pRefObj != p )
		{
			Reset( p );
		}
		return *this;
	}

	template<class Tother>
	operator Tother*() const
	{
		return reinterpret_cast<Tother*>(m_pRefObj);
	}

	operator T*() const
	{
		return m_pRefObj;
	}

	T& operator*() const
	{
		return *m_pObject;
	}

	T* operator ->() const
	{
		_ASSERT( m_pRefObj != NULL );
		return m_pRefObj;
	}

	bool operator ==( T* pObject ) const
	{
		return m_pRefObj == pObject;
	}

	bool operator !=( T* pObject ) const
	{
		return m_pRefObj != pObject;
	}

	bool operator ==( const sptr& ptr ) const
	{
		return m_pRefObj == ptr.m_pRefObj;
	}

	bool operator !=( const sptr& ptr ) const
	{
		return m_pRefObj != ptr.m_pRefObj;
	}

private:
	T*	m_pRefObj;
};
}