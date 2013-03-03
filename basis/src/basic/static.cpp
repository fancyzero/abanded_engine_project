#include "stdafx.h"


namespace mania
{
critical_section identifier::cs;
std::map<string, int> identifier::counter;
classinfo object::static_classinfo;

object::object()
		: m_classinfo( null ), m_outer( NULL ), m_tag( 0 )
{
	MANIA_CLASS_CONSTRUCTOR_INCLUDE( object );
}

object::~object()
{
}

bool object::is( const classinfo* aclassinfo )
{
	return match_classinfo( aclassinfo );
}

bool object::match_classinfo( const classinfo* aclassinfo )
{
	return m_classinfo->is( aclassinfo );
}
}