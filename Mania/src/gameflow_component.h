#pragma once

class gameflow_component :
		public mania::core_component
{
public:
	gameflow_component( void );
	~gameflow_component( void );

	virtual DWORD OnUpdate();
	virtual DWORD OnEndFrame();
};
