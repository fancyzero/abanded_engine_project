#pragma once

namespace mania
{
class indexbuffer : public render_resource
{
public:
	struct desc_t
	{
		desc_t() {};
		desc_t( UINT a_size, eformat a_format, eusage a_usage, epool a_pool )
				: size( a_size ), format( a_format ), usage( a_usage ), pool( a_pool ), type( rtype_indexbuffer ) {};
		UINT			size;
		eformat			format;
		eusage			usage;
		epool			pool;
		eresourcetype	type;
	};
	indexbuffer( sptr<renderdevice> renderdevice ): render_resource( renderdevice ) {}
	virtual DWORD	set_desc( const desc_t& indesc ) { m_desc = indesc; return grc_succeeded; }
	virtual DWORD	get_desc( desc_t& outdesc ) { outdesc = m_desc; return grc_succeeded; }
	virtual DWORD	lock( UINT lockstart, UINT locksize, void** lockeddata, DWORD flags )	= 0;
	virtual DWORD	unlock() = 0;

	virtual DWORD	create( const desc_t& desc )										= 0;
	virtual DWORD	destroy()															= 0;
protected:
	desc_t		m_desc;
};
}