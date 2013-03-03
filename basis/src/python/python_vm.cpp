#include "stdafx.h"
#include <Python.h>
#include "Python_vm.h"
namespace mania
{
	python_vm::python_vm()
	{
	}

	python_vm::~python_vm()
	{
		finalize();
	}
	void python_vm::initialize()
	{
		Py_SetPythonHome(".\\Python");
		Py_Initialize();
	}

	void python_vm::finalize()
	{
		Py_Finalize();
	}

	void python_vm::test()
	{
	}

	gresult python_vm::execute_script( const TCHAR* script )
	{
		std::size_t converted = 0;
		std::size_t max_size = wcslen( script )*2+2;
		char* converted_string = new char[max_size];
		wcstombs_s( &converted, converted_string, max_size, script, max_size );
		PyRun_SimpleString( converted_string );
		return grc_succeeded;
	}
};