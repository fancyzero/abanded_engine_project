#pragma once

namespace mania
{
class d3d9indexbuffer;
typedef resource_object< d3d9indexbuffer*, IDirect3DIndexBuffer9*> d3d9indexbuffer_res_base;
class d3d9indexbuffer_res : public d3d9indexbuffer_res_base
{
public:
	d3d9indexbuffer_res( d3d9indexbuffer* host ): resource_object( host ) {}

public:
	DWORD create( const indexbuffer::desc_t& desc );
	DWORD destroy();
};

class d3d9indexbuffer : public indexbuffer
{
	friend class d3d9renderdevice;
protected:
	d3d9indexbuffer( renderdevice_ptr renderdevice ): indexbuffer( renderdevice ) {}
	virtual ~d3d9indexbuffer( void );
public:
	virtual DWORD	create( const indexbuffer::desc_t& desc );
	virtual DWORD	lock( UINT lockstart, UINT locksize, void** lockeddata, DWORD flags );
	virtual DWORD	unlock();
	virtual DWORD	destroy();
	virtual gresult	on_lost_device();
	virtual gresult	on_reset_device();
private:
	sptr<d3d9indexbuffer_res>	m_d3d9indexbuffer_res;

};
}