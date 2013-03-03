#pragma once

namespace mania
{
class d3d9vertexbuffer;
typedef resource_object< d3d9vertexbuffer*, IDirect3DVertexBuffer9*> d3d9vertexbuffer_res_base;
class d3d9vertexbuffer_res : public d3d9vertexbuffer_res_base
{
	friend class d3d9renderdevice;
public:
	d3d9vertexbuffer_res( d3d9vertexbuffer* host ): resource_object( host ) {}
public:
	DWORD create( const vertexbuffer::desc_t& desc );
	DWORD destroy();
};

class d3d9vertexbuffer : public vertexbuffer
{
	friend class d3d9renderdevice;
protected:
	d3d9vertexbuffer( renderdevice_ptr renderdevice ): vertexbuffer( renderdevice ) {}
	virtual ~d3d9vertexbuffer( void );
public:
	virtual DWORD	create( const vertexbuffer::desc_t& desc );
	virtual DWORD	lock( UINT lockstart, UINT locksize, void** lockeddata, DWORD flags );
	virtual DWORD	unlock();
	virtual DWORD	destroy();
	virtual gresult	on_lost_device();
	virtual gresult	on_reset_device();
private:
	sptr<d3d9vertexbuffer_res>	m_d3d9vertexbuffer_res;
};
}