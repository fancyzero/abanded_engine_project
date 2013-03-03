#pragma once
namespace mania
{
class d3d9font;
typedef resource_object< d3d9font*, ID3DXFont*> d3dfont_res_base;
class d3d9font_res : public d3dfont_res_base
{
	friend class d3d9font;
public:
	d3d9font_res( d3d9font* host )
			: resource_object( host )
	{
	}
public:
	DWORD create( const string& url );
	DWORD destroy();
};

class d3d9font : public font
{
public:
	//MANIA_DECLARE_CLASS(d3d9font)
public:
	d3d9font( sptr<d3d9renderdevice> renderdevice )
			: font( renderdevice )
	{
	}
	~d3d9font( void );
	DWORD create( const string& url );
	DWORD destroy();
private:
	sptr<d3d9font_res>	m_d3dfont_res;
};
}