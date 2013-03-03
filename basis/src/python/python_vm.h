#pragma once

namespace mania
{
class MANIA_BASIS_API python_vm
{
public:
	python_vm();
	~python_vm();
	void	initialize();
	void    finalize();
	void	test();
	gresult	execute_script( const TCHAR* script );
};
};
