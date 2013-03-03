#pragma once

namespace mania
{
	
class MANIA_ENGINE_API console
{
public:
	console( void );
	~console( void );

	BOOL init();

	static console* create();

	void write( TCHAR* );
	void clear();

private:
	BOOL start_input_thread();
	BOOL start_output_thread();
	class console_input_thread*		m_input_thread;
	class console_output_thread*	m_output_thread;
};

}