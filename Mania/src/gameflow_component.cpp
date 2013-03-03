#include "StdAfx.h"
#include "gameflow_component.h"

gameflow_component::gameflow_component( void )
{
}

gameflow_component::~gameflow_component( void )
{
}

DWORD gameflow_component::OnUpdate()
{
	return mania::ccrc_continue;
}

DWORD gameflow_component::OnEndFrame()
{
	return mania::ccrc_continue;
}