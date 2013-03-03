#pragma once

namespace mania
{
class renderdevice;
class render_resource : public resource
{
public:
	render_resource( sptr<renderdevice> renderdevice )
			: m_renderdevice( renderdevice )
	{
	}
	virtual ~render_resource( void )
	{
	}
	sptr<renderdevice>	get_renderer()
	{
		return m_renderdevice;
	}
	virtual gresult		on_lost_device() = 0;
	virtual gresult		on_reset_device() = 0;
protected:
	sptr<renderdevice>	m_renderdevice;
};
}