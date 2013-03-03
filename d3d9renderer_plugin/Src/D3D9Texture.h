#pragma once

namespace mania
{
class d3d9renderdevice;
typedef resource_object< d3d9texture*, IDirect3DTexture9*> d3d9texture_res_base;
class d3d9texture_res : public d3d9texture_res_base
{
	friend class d3d9texture;
public:
	d3d9texture_res( d3d9texture* host ): resource_object( host ) {}
	struct create_param
	{
		int w;
		int h;
		int mip_count;
		int usage;
		int format;
		int pool;
	};

	struct create_by_url_param
	{
		string url;
		int mip_count;
		int usage;
		int format;
		int pool;
	};

	struct create_by_mem_param
	{
		void* dataaddr;
		DWORD datasize;
		int mip_count;
		int usage;
		int format;
		int pool;
	};

public:
	DWORD create( int w, int h, int mip_count, int usage, int format, int pool );
	DWORD create( const string& url, int mip_count, int usage, int format, int pool );
	DWORD create( const void* dataaddr, DWORD datasize, int mip_count, int usage, int format, int pool );

	DWORD destroy();
};

class d3d9texture : public texture
{
	friend class d3d9renderdevice;
	friend class d3d9effect;
protected:
	d3d9texture( d3d9renderer_ptr renderdevice );
	virtual ~d3d9texture( void );
public:
	virtual void get_size( int &w, int &h ) const
	{
		w = m_width;
		h = m_height;
	}
	virtual DWORD get_format() const {return m_format;}

protected:
	virtual DWORD create( int w, int h, int mip_count, int usage, int format, int pool );
	virtual DWORD create( const string& url, int mip_count, int usage, int format, int pool );
	virtual DWORD create( const void* dataaddr, DWORD datasize, int mip_count, int usage, int format, int pool );
	virtual DWORD destroy();
	virtual DWORD get_surface( int level, IDirect3DSurface9 ** surface9 );
	virtual gresult	on_lost_device();
	virtual gresult	on_reset_device();
private:
	int								m_width;
	int								m_height;
	DWORD							m_format;
	sptr<d3d9texture_res>	m_texres;
};
}