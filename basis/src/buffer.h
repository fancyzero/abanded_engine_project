#pragma once

namespace mania
{
class MANIA_BASIS_API buffer : public object
{
	MANIA_DECLAR_CLASS( buffer );
public:
	buffer();
	buffer( void* data, unsigned int size );
	~buffer( void );

	unsigned int	get_size()
	{
		return m_size;
	}
	void*			get_data()
	{
		return m_data;
	}
private:
	unsigned int	m_size;
	void*			m_data;
};

}