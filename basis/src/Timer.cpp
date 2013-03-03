#include "stdafx.h"
#include "Common.h"
#include "timer.h"
//#include "dxutil.h"

namespace mania
{
#define GETTIMESTAMP timeGetTime
enum TIMER_COMMAND { TIMER_RESET, TIMER_START, TIMER_STOP, TIMER_ADVANCE,
                     TIMER_GETABSOLUTETIME, TIMER_GETAPPTIME, TIMER_GETELAPSEDTIME
                   };
//-----------------------------------------------------------------------------
// Name: DXUtil_Timer()
// Desc: Performs timer opertations. Use the following commands:
//          TIMER_RESET           - to reset the timer
//          TIMER_START           - to start the timer
//          TIMER_STOP            - to stop (or pause) the timer
//          TIMER_ADVANCE         - to advance the timer by 0.1 seconds
//          TIMER_GETABSOLUTETIME - to get the absolute system time
//          TIMER_GETAPPTIME      - to get the current time
//          TIMER_GETELAPSEDTIME  - to get the time that elapsed between
//                                  TIMER_GETELAPSEDTIME calls
//-----------------------------------------------------------------------------
double __stdcall DXUtil_Timer( TIMER_COMMAND command )
{
	static BOOL     m_bTimerInitialized = FALSE;
	static BOOL     m_bUsingQPF         = FALSE;
	static BOOL     m_bTimerStopped     = TRUE;
	static LONGLONG m_llQPFTicksPerSec  = 0;
	// Initialize the timer
	if ( FALSE == m_bTimerInitialized )
	{
		m_bTimerInitialized = TRUE;
		// Use QueryPerformanceFrequency() to get frequency of timer.  If QPF is
		// not supported, we will timeGetTime() which returns milliseconds.
		LARGE_INTEGER qwTicksPerSec;
		m_bUsingQPF = QueryPerformanceFrequency( &qwTicksPerSec );
		if ( m_bUsingQPF )
			m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
	}
	if ( m_bUsingQPF )
	{
		static LONGLONG m_llStopTime        = 0;
		static LONGLONG m_llLastElapsedTime = 0;
		static LONGLONG m_llBaseTime        = 0;
		double fTime;
		double fElapsedTime;
		LARGE_INTEGER qwTime;
		// Get either the current time or the stop time, depending
		// on whether we're stopped and what command was sent
		if ( m_llStopTime != 0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME )
			qwTime.QuadPart = m_llStopTime;
		else
			QueryPerformanceCounter( &qwTime );
		// Return the elapsed time
		if ( command == TIMER_GETELAPSEDTIME )
		{
			fElapsedTime = ( double )( qwTime.QuadPart - m_llLastElapsedTime ) / ( double ) m_llQPFTicksPerSec;
			m_llLastElapsedTime = qwTime.QuadPart;
			return  fElapsedTime;
		}
		// Return the current time
		if ( command == TIMER_GETAPPTIME )
		{
			double fAppTime = ( double )( qwTime.QuadPart - m_llBaseTime ) / ( double ) m_llQPFTicksPerSec;
			return  fAppTime;
		}
		// Reset the timer
		if ( command == TIMER_RESET )
		{
			m_llBaseTime        = qwTime.QuadPart;
			m_llLastElapsedTime = qwTime.QuadPart;
			m_llStopTime        = 0;
			m_bTimerStopped     = FALSE;
			return 0.0;
		}
		// Start the timer
		if ( command == TIMER_START )
		{
			if ( m_bTimerStopped )
				m_llBaseTime += qwTime.QuadPart - m_llStopTime;
			m_llStopTime = 0;
			m_llLastElapsedTime = qwTime.QuadPart;
			m_bTimerStopped = FALSE;
			return 0.0;
		}
		// Stop the timer
		if ( command == TIMER_STOP )
		{
			if ( !m_bTimerStopped )
			{
				m_llStopTime = qwTime.QuadPart;
				m_llLastElapsedTime = qwTime.QuadPart;
				m_bTimerStopped = TRUE;
			}
			return 0.0;
		}
		// Advance the timer by 1/10th second
		if ( command == TIMER_ADVANCE )
		{
			m_llStopTime += m_llQPFTicksPerSec / 10;
			return 0.0;
		}
		if ( command == TIMER_GETABSOLUTETIME )
		{
			fTime = qwTime.QuadPart / ( double ) m_llQPFTicksPerSec;
			return  fTime;
		}
		return -1.0f; // Invalid command specified
	}
	else
	{
		// Get the time using timeGetTime()
		static double m_fLastElapsedTime  = 0.0;
		static double m_fBaseTime         = 0.0;
		static double m_fStopTime         = 0.0;
		double fTime;
		double fElapsedTime;
		// Get either the current time or the stop time, depending
		// on whether we're stopped and what command was sent
		if ( m_fStopTime != 0.0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME )
			fTime = m_fStopTime;
		else
		{
			timeBeginPeriod( 1 );
			fTime = GETTIMESTAMP() * 0.001;
			timeEndPeriod( 1 );
		}
		// Return the elapsed time
		if ( command == TIMER_GETELAPSEDTIME )
		{
			fElapsedTime = ( double )( fTime - m_fLastElapsedTime );
			m_fLastElapsedTime = fTime;
			return ( FLOAT ) fElapsedTime;
		}
		// Return the current time
		if ( command == TIMER_GETAPPTIME )
		{
			return ( fTime - m_fBaseTime );
		}
		// Reset the timer
		if ( command == TIMER_RESET )
		{
			m_fBaseTime         = fTime;
			m_fLastElapsedTime  = fTime;
			m_fStopTime         = 0;
			m_bTimerStopped     = FALSE;
			return 0.0;
		}
		// Start the timer
		if ( command == TIMER_START )
		{
			if ( m_bTimerStopped )
				m_fBaseTime += fTime - m_fStopTime;
			m_fStopTime = 0.0f;
			m_fLastElapsedTime  = fTime;
			m_bTimerStopped = FALSE;
			return 0.0;
		}
		// Stop the timer
		if ( command == TIMER_STOP )
		{
			if ( !m_bTimerStopped )
			{
				m_fStopTime = fTime;
				m_fLastElapsedTime  = fTime;
				m_bTimerStopped = TRUE;
			}
			return 0.0;
		}
		// Advance the timer by 1/10th second
		if ( command == TIMER_ADVANCE )
		{
			m_fStopTime += 0.1f;
			return 0.0;
		}
		if ( command == TIMER_GETABSOLUTETIME )
		{
			return  fTime;
		}
		return -1.0f; // Invalid command specified
	}
}


timer::timer( void )
{
	ZeroMemory( &m_last, sizeof( m_last ) );
	ZeroMemory( &m_now, sizeof( m_now ) );
	ZeroMemory( &m_freq, sizeof( m_freq ) );
	m_lag = 0.0;
	DXUtil_Timer( TIMER_START );
	m_paused = FALSE;
	m_elapsed = DXUtil_Timer( TIMER_GETAPPTIME );
	m_last_abslout_time = m_abslout_time = DXUtil_Timer( TIMER_GETABSOLUTETIME );
};

timer::~timer( void )
{
}

void timer::update()
{
	m_lag = DXUtil_Timer( TIMER_GETELAPSEDTIME );//(m_Now.QuadPart - m_Last.QuadPart)/(float)m_Freq.QuadPart ;
	m_elapsed = DXUtil_Timer( TIMER_GETAPPTIME );
	m_last_abslout_time = m_abslout_time;
}

void timer::pause()
{
	if ( m_paused == TRUE )
		return;
	m_paused = TRUE;
	DXUtil_Timer( TIMER_STOP );
}

void timer::unpause()
{
	if ( m_paused == FALSE )
		return;
	m_paused = FALSE;
	DXUtil_Timer( TIMER_START );
}
}