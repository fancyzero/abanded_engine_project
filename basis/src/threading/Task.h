#pragma once

namespace mania
{
class MANIA_BASIS_API task
{
public:
	task( void ) {};
	virtual ~task( void ) {};
	virtual DWORD execute() = 0;
};
}