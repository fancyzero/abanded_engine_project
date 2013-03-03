#include "StdAfx.h"
#include "tnode.h"

namespace mania
{
	MANIA_IMPLEMENT_CLASS( tnode );
	tnode::tnode( void )
		:node()
	{
		MANIA_CLASS_CONSTRUCTOR_INCLUDE( tnode );
		m_local_transform.set_identity();
	}

	tnode::~tnode( void )
	{
	}

	const transform& tnode::get_world_transform()
	{
		update_world_transform();
		return m_world_transform;
	}
	
	void tnode::update_world_transform()
	{
		if ( get_parent() == NULL )
			m_world_transform = m_local_transform;
		else
		{
			m_world_transform = m_local_transform;
			m_world_transform.transformed_by(((sptr<tnode>)get_parent())->get_world_transform());
		}
	}
}