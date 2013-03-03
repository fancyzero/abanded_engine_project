#pragma once

namespace mania
{
using namespace math;
class MANIA_ENGINE_API transform
{
public:
	transform( void )
	{}
	~transform( void )
	{}

	const math::matrix44&	get_matrix() const
	{
		if ( m_matrix_dirty ) 
			update_matrix();
		return m_matrix;
	}
	const math::vector3&	get_scale() const
	{
		if ( m_scale_dirty ) update_scale();
		return m_scale;
	}
	const math::vector3&	get_rotation() const
	{
		if ( m_rotation_dirty ) update_rotation();
		return m_rotation;
	}
	
	const math::vector3&	get_translation() const
	{
		if ( m_translation_dirty ) update_translation();
		return m_translation;
	}

	void set_matrix( const matrix44& amat )
	{
		m_matrix = amat;
		m_dirty_bits |= 2 | 4 | 8;
	}
	void set_scale( const vector3& ascale )
	{
		m_scale = ascale;
		m_dirty_bits |= 1;
	}
	void set_rotation( float x, float y, float z )
	{
		m_rotation = vector3(x,y,z);
		m_dirty_bits |= 1;
	}
	void set_rotation( const vector3& arotation )
	{
		m_rotation = arotation;
		m_dirty_bits |= 1;
	}

	void set_translation( const vector3& atranslation )
	{
		m_translation = atranslation;
		m_dirty_bits |= 1;
	}

	void set_identity()
	{
		m_dirty_bits = 0;
		m_scale.x = m_scale.y = m_scale.z = 1.0f;
		m_rotation.x = m_rotation.y = m_rotation.z = 0.0f;
		m_translation.x = m_translation.y = m_translation.z = 0.0f;
		ZeroMemory( m_matrix.m, sizeof (m_matrix.m) );
		m_matrix._11 = m_matrix._22 = m_matrix._33 = m_matrix._44 = 1.0f;
	}

	void transformed_by( const transform& t )
	{
		//perform self = t*self
		math::matrix44& self_mat = _get_matrix();
		const math::matrix44& t_mat = t.get_matrix();
		set_matrix( self_mat * t_mat  );
	}

	void update_matrix() const;
	void update_scale() const;
	void update_rotation() const;
	void update_translation() const;

	//utility functions
	//math::vector3* rotate( const math::vector3* v )
	//{
	//	const math::matrix44& = get_matrix();
	//}
	
protected:

	math::matrix44&	_get_matrix()
	{
		if ( m_matrix_dirty ) update_matrix();
		return m_matrix;
	}
	mutable math::matrix44	m_matrix;
	mutable math::vector3	m_scale;
	mutable math::vector3	m_rotation;
	mutable math::vector3	m_translation;
	union
	{
		mutable unsigned int m_dirty_bits;
		struct
		{
			mutable unsigned	m_matrix_dirty: 1;
			mutable unsigned	m_scale_dirty: 1;
			mutable unsigned	m_rotation_dirty: 1;
			mutable unsigned	m_translation_dirty: 1;
		};
	};
};
}