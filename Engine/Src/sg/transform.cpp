#include "StdAfx.h"
#include "transform.h"

namespace mania
{
	void transform::update_matrix() const
	{
		m_matrix_dirty = 0;
		build_matrix_trans_rot_scale( &m_matrix, m_translation, m_rotation, m_scale );
		//update_rotation();
	}

	void transform::update_scale() const
	{
		m_scale_dirty = 0;
		m_scale.x = m_matrix.get_axis( axis_x ).length();
		m_scale.y = m_matrix.get_axis( axis_y ).length();
		m_scale.z = m_matrix.get_axis( axis_z ).length();
	}

	void transform::update_rotation() const
	{
		m_rotation_dirty = 0;
		m_rotation.x = arctan2radian( m_matrix._23, sqrtf(m_matrix._33*m_matrix._33 + m_matrix._13*m_matrix._13) );
		m_rotation.y = arctan2radian( -m_matrix._13, m_matrix._33 );
		m_rotation.z = arctan2radian( -m_matrix._21, m_matrix._22 );
	}

	void transform::update_translation() const
	{
		m_translation_dirty = 0;
		m_translation = vector3(m_matrix._41, m_matrix._42, m_matrix._43 );
	}
}