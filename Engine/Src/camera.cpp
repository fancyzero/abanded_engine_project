#include "StdAfx.h"
#include "camera.h"

namespace mania
{
	mania::classinfo camera::static_classinfo;
	camera::camera()
	:tnode()
	{
		MANIA_CLASS_CONSTRUCTOR_INCLUDE( camera );
	}

	camera::~camera()
	{
	}

	void camera::update( float lag )
	{
		tnode::update( lag );
		const math::matrix44 mat = m_local_transform.get_matrix();
		math::vector3 eye(0,0,0), lookat(0,0,1);
		math::vec_transform( &eye, &mat );
		math::vec_transform( &lookat, &mat );
		math::matrix44 viewtransform;
		math::build_matrix_lookat_lh( &viewtransform, &eye, &lookat, &vector3(0,1,0) );
		get_core()->get_scene_renderer()->set_viewtransform(viewtransform);
	}
}