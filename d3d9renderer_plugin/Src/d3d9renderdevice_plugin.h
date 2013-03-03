namespace mania
{
#ifdef D3D9RENDERDEVICE_DLL_EXPORT
#define D3D9RD_DLL_API __declspec(dllexport)
#else
#define D3D9RD_DLL_API __declspec(dllimport)
#endif

extern "C"
{
	D3D9RD_DLL_API mania::plugin* create_plugin();
};

class d3d9renderdevice_plugin : public renderdevice_plugin
{
public:
	virtual void get_desc( plugin_desc &desc );
	virtual sptr<class renderdevice> create_renderer();
};
}