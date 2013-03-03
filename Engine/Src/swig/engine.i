 %module core

class core
{
public:

	DWORD	init( HINSTANCE hInst, HWND hWnd );
	DWORD	uninit();
	void	BeginFrame();
	void	update();
	void	EndFrame();
	void	BeginVisualFrame();
	void	UpdateVisual();
	void	EndVisualFrame();
	void	BeginLogicalFrame();
	void	UpdateLogical();
	void	EndLogicalFrame();

};

