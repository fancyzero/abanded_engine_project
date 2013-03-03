#pragma once


using namespace mania;

class drawer
{
public:
	drawer( sptr<renderdevice> renderdevice )
			: m_renderdevice( renderdevice )
	{
	}
	drawer();
	~drawer( void );

	DWORD init( sptr<renderdevice> );
	DWORD uninit();
	DWORD draw_widget( sptr<widget> wdg );
	DWORD draw_staticmesh( sptr<staticmesh> sm );

public:
	sptr<renderdevice> m_renderdevice;
	sptr<mania::vertex_declaration> m_simple_vertexdecl;
	sptr<mania::vertexbuffer>		m_simple_vb;
	sptr<mania::indexbuffer>		m_simple_ib;
};
