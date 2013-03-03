#pragma once

//utility functions
namespace mania
{

DWORD MANIA_BASIS_API init_stringtable( const string& filename );
const MANIA_BASIS_API string& get_string( const string& idstring );

MANIA_BASIS_API string convert_to_xmlurl( const string& url );
}