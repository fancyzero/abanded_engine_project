#pragma once

using namespace mania;
class widget : public object
{
	MANIA_DECLAR_CLASS( widget )
public:
	widget( void );
	~widget( void );

	DWORD create( string effect_url, string texture_url );
	DWORD destroy();

	float m_x;
	float m_y;
	float m_w;
	float m_h;

	sptr<effect> m_effect;
	sptr<texture> m_texture;
};
