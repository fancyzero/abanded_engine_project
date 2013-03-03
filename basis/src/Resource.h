#pragma once

namespace mania
{
class MANIA_BASIS_API resource : public object
{
	MANIA_DECLAR_CLASS( resource );
public:
	enum LoadState
	{
		rlsUnload,
		rlsLoading,
		rlsLoaded,
	};

	resource();
	virtual ~resource( void ) {};

	inline
	DWORD getLoadState() { return m_loadstate; }
	void set_url( const string& url ) { m_url = url; }
	const string& get_url() const { return m_url; }
protected:
	string		m_url;
	DWORD		m_loadstate;
};

template< class hostclass, class objclass >
class resource_object : public object
{
	MANIA_DECLAR_CLASS( resource_object );
public:
	resource_object()
			: m_host( null )
	{
		MANIA_CLASS_CONSTRUCTOR_INCLUDE( resource_object );
	}
	resource_object( hostclass host )
			: m_host( host )
	{
		MANIA_CLASS_CONSTRUCTOR_INCLUDE( resource_object );
	}
	hostclass	get_host() { return m_host; }
	objclass	get_object() { return m_object; }
protected:
	objclass					m_object;
	hostclass					m_host;
};
}