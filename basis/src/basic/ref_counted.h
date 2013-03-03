#pragma once

namespace mania
{
class MANIA_BASIS_API ref_counted
{
public:
	ref_counted(): m_count( 0 ) {}
	virtual ~ref_counted() {}

	UINT addref() { return InterlockedIncrement( &m_count );}

	UINT release()
	{
		LONG cnt = InterlockedDecrement( &m_count );
		_ASSERT( cnt >= 0 );
		if ( cnt == 0 )
			delete_this();
		return cnt;
	}
	UINT get_refcount()
	{
		InterlockedIncrement( &m_count );
		return InterlockedDecrement( &m_count );
	}

protected:
	virtual void delete_this();

private:
	LONG m_count;
};
}