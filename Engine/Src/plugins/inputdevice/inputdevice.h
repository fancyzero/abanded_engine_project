#pragma once

namespace mania
{
class inputdevice_plugin_impl : public inputdevice_plugin
{
public:
	inputdevice_plugin_impl()
			: m_inputdevice( NULL )
	{
	}
	virtual void get_desc( plugin_desc& desc );
	virtual inputdevice* get_inputdevice();
private:
	class inputdevice_win*	m_inputdevice;
};

struct input_state
{
	BYTE key_states[300];
	int mouse_states[16];//[0] xaxis, [1] yaxis, [2] wheel
};

class MANIA_ENGINE_API inputdevice : public object
{
public:
	virtual gresult update_state() = 0;
	virtual gresult get_input_state( input_state* state ) = 0;
};

class inputdevice_win : public inputdevice
{
public:
	gresult update_state();
	gresult get_input_state( input_state* state );
private:
	input_state	m_state;
};
}