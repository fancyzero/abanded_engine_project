#pragma once

namespace mania
{

class texture : public render_resource
{
public:
	texture( renderdevice_ptr renderdevice ): render_resource( renderdevice ) {}
	virtual void				get_size( int &w, int &h ) const = 0;
	virtual DWORD				get_format() const = 0;
	virtual DWORD	destroy()									= 0;
};
}