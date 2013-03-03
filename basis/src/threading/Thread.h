#pragma once

namespace mania
{
class MANIA_BASIS_API Thread
{
public:

	Thread( void );
	virtual ~Thread( void );

	BOOL	Begin( void* pParamm, BOOL bStartSuspended );
	BOOL	Pause();
	BOOL	Resume();
	BOOL	KillSoftly( DWORD dwTime );
	BOOL	Kill( DWORD dwCode );
	DWORD	get_id() { return m_dwThreadID; }
	DWORD	wait( DWORD mseconds );

	virtual DWORD OnBegin( void* userparam ) { return grc_succeeded; }
	virtual DWORD OnLoop( void* userparam ) { return grc_succeeded; }
	virtual DWORD OnEnd( void* userparam ) { return grc_succeeded; }
	virtual void OnBeginKill() {};

	void	SetThreadProcAddress( LPTHREAD_START_ROUTINE pThreadProcAddr )
	{
		m_pTreadProcAddress = pThreadProcAddr;
	}

	struct ThreadInitParam_T
	{
		Thread*	pThread;
		void*	pUserParam;
	};

	HANDLE	GetThreadTermEvent()
	{
		return m_hThreadTermEvent;
	}

	static DWORD default_proc( void* param );
protected:
	LPTHREAD_START_ROUTINE m_pTreadProcAddress;
	void*		m_pThreadParam;
private:
	DWORD		m_dwThreadID;
	HANDLE		m_hThreadHandle;
	HANDLE		m_hThreadTermEvent;
};
}