#pragma once

namespace mania
{
class renderdevice;
class MANIA_ENGINE_API rendering_thread :
		public Thread
{
public:
	rendering_thread( void );
	virtual ~rendering_thread( void );

	BOOL				Init( HWND hWnd );
	BOOL				Uninit();
	virtual DWORD		Loop();
	virtual void		OnBegin();
	virtual void		OnBeginKill();
	virtual void		OnEnd();

	renderdevice_ptr get_renderdevice() const
	{
		return m_renderdevice;
	}

	//Tasks
	BOOL	AddRenderTask( task* pTask );
	BOOL	flush_all_task();
	void	next_frame();
protected:
	int						m_frame_cnt;
	taskqueue_manager		m_rendertask_queue;
	renderdevice_ptr		m_renderdevice;
};
}