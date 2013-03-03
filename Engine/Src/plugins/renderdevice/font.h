#pragma once

namespace mania
{
class font : public render_resource
{
public:
	//MANIA_DECLAR_CLASS(font);
public:
	font( renderdevice_ptr renderdevice )
			: render_resource( renderdevice )
	{
	}
	virtual gresult draw( float x, float y, const string& text ) = 0;
	virtual DWORD create( const string& url ) = 0;
	virtual DWORD destroy() = 0;
};
}