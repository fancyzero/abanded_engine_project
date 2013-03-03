#pragma once

namespace mania
{
class vertexbuffer : public render_resource
{
public:
	struct desc_t
	{
		desc_t( UINT a_size, eusage a_usage, epool a_pool, UINT a_stride )
				: size( a_size ), usage( a_usage ), pool( a_pool ), stride( a_stride ) {};
		desc_t() {};
		UINT			size;//in byte
		eusage			usage;
		epool			pool;
		UINT			stride;//in byte
	};

	vertexbuffer( sptr<renderdevice> renderdevice ): render_resource( renderdevice ) {}
	virtual DWORD	set_desc( const desc_t& desc ) { m_desc = desc; return grc_succeeded; }
	virtual DWORD	get_desc( desc_t& desc ) { desc = m_desc; return grc_succeeded; }
	UINT	get_stride() { return m_desc.stride; }
	virtual DWORD	create( const desc_t& desc )											= 0;
	virtual DWORD	lock( UINT lockstart, UINT locksize, void** lockeddata, DWORD flags )	= 0;
	virtual DWORD	unlock()																= 0;
	virtual DWORD	destroy()																= 0;
protected:
	desc_t	m_desc;
};
}