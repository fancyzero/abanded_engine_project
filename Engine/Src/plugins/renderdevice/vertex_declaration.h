#pragma once

namespace mania
{
class vertex_declaration : public object
{
public:
	vertex_declaration() {};
	~vertex_declaration() {};
	virtual DWORD destroy() = 0;
};
}