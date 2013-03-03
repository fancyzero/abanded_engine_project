#pragma once

namespace mania
{
class MANIA_BASIS_API timer
{

public:
	timer( void );
	~timer( void );

	void pause();
	void unpause();
	BOOL is_paused() { return m_paused; }
	double get_elapsed() { return m_elapsed; }
	double get_abslout() { return m_abslout_time; }
	double get_last_abslout() { return m_last_abslout_time; }
	double get_lag() { return m_lag; }
	void update();
private:
	LARGE_INTEGER	m_last;
	LARGE_INTEGER	m_now;
	LARGE_INTEGER	m_freq;
	double			m_lag;
	double			m_elapsed;
	double			m_abslout_time;
	double			m_last_abslout_time;
	BOOL			m_paused;
};
}