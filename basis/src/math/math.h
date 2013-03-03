#include <math.h>
#include <emmintrin.h>
#pragma once

namespace mania
{
namespace math
{
//data types
enum e_axis_type
{
	axis_x,
	axis_y,
	axis_z,
};


// integer struct
struct point
{
	int x;
	int y;
};
//vector2
struct MANIA_BASIS_API vector2
{
	vector2()
			: x( 0 ), y( 0 )
	{}

	vector2( float Ax, float Ay )
			: x( Ax ), y( Ay )
	{}

	vector2( float a[2] )
			: x( a[0] ), y( a[1] )
	{}

	vector2& operator =( float a[2] )
	{
		x = a[0];
		y = a[1];
		return *this;
	}

	bool operator == ( const vector2& v ) const
	{
		return ( v.x == x ) && ( v.y == y );
	}

	vector2& operator = ( const vector2& v )
	{
		x = v.x;
		y = v.y;
		return *this;
	}
	operator float*()
	{
		return &x;
	}

	float x;
	float y;
};



//vector3
struct MANIA_BASIS_API vector3
{
	vector3()
			: x( 0 ), y( 0 ), z( 0 )
	{
	}

	vector3( float Ax, float Ay, float Az )
			: x( Ax ), y( Ay ), z( Az )
	{
	}

	vector3( float a[3] )
			: x( a[0] ), y( a[1] ), z( a[2] )
	{
	}
	vector3& operator =( float a[3] )
	{
		x = a[0];
		y = a[1];
		z = a[2];
		return *this;
	}
	vector3 operator + ( const vector3& v ) const
	{
		return vector3( x + v.x, y + v.y, z + v.z );
	}
	vector3 operator - ( const vector3& v ) const
	{
		return vector3( x - v.x, y - v.y, z - v.z );
	}
	vector3 operator * ( float f ) const
	{
		return vector3( x * f, y * f, z * f );
	}
	void normalize()
	{
		float r = sqrtf( x * x + y * y + z * z );
		x = x / r;
		y = y / r;
		z = z / r;
	}

	bool operator == ( const vector3& v ) const
	{
		return ( v.x == x ) && ( v.y == y ) && ( v.z == z );
	}

	vector3& operator = ( const vector3& v )
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	float length() const
	{
		return sqrt( x*x + y*y + z*z );
	}

	operator float*()
	{
		return &x;
	}


	float x;
	float y;
	float z;


};

//vector4
struct MANIA_BASIS_API vector4
{

	vector4()
			: x( 0 ), y( 0 ), z( 0 ), w( 0 )
	{
	}

	vector4( float Ax, float Ay, float Az, float Aw )
			: x( Ax ), y( Ay ), z( Az ), w( Aw )
	{
	}

	vector4( float a[4] )
			: x( a[0] ), y( a[1] ), z( a[2] ), w( a[3] )
	{
	}

	vector4& operator =( float a[4] )
	{
		x = a[0];
		y = a[1];
		z = a[2];
		w = a[3];
		return *this;
	}
	operator mania::math::vector3()
	{
		return vector3( x, y, z );
	}

	operator float*()
	{
		return &x;
	}

	float length() const
	{
		return sqrt( x*x + y*y + z*z + w*w );
	}

	void normalize()
	{
		float r = sqrtf( x * x + y * y + z * z + w * w );
		x = x / r;
		y = y / r;
		z = z / r;
		w = w / r;
	}

	float x;
	float y;
	float z;
	float w;

};
//


struct Edge2
{
	vector2 p[2];
};

struct Edge3
{
	vector3 p[2];
};
//matrix44
struct MANIA_BASIS_API matrix44;
MANIA_BASIS_API matrix44* multiply_matrix( matrix44* out, const matrix44* m1, const matrix44* m2 );
MANIA_BASIS_API void inverse_matrix( matrix44* m );

struct MANIA_BASIS_API matrix44
{

	matrix44()
	{
		ZeroMemory( m, sizeof m );
	}
	matrix44(	float m11, float m12, float m13, float m14,
	          float m21, float m22, float m23, float m24,
	          float m31, float m32, float m33, float m34,
	          float m41, float m42, float m43, float m44	)
			: _11( m11 ), _12( m12 ), _13( m13 ), _14( m14 ),
			_21( m21 ), _22( m22 ), _23( m23 ), _24( m24 ),
			_31( m31 ), _32( m32 ), _33( m33 ), _34( m34 ),
			_41( m41 ), _42( m42 ), _43( m43 ), _44( m44 )
	{
	}

	matrix44 operator * ( const matrix44& right ) const
	{
		matrix44 result;
		multiply_matrix( &result, this, &right );
		return result;
	}

	void set_identity()
	{
		ZeroMemory( m, sizeof m );
		_11 = _22 = _33 = _44 = 1.0f;
	}

	vector4 get_axis( e_axis_type axis )
	{
		return m[axis];
	}
	operator float*() 
	{
		return &m[0][0];
	}
	operator const float*() const
	{
		return &m[0][0];
	}
	void inverse()
	{
		inverse_matrix(this);
	}
	union
	{
		struct
		{
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float		m[4][4];
	};
};

struct MANIA_BASIS_API Color
{
	Color( DWORD dwColor )
	{
		*this = dwColor;
	}
	Color()
	{
		ARGB = ::_mm_setzero_ps();
	}

	union
	{
		__m128 ARGB;
		struct
		{
			float a;
			float r;
			float g;
			float b;
		};
	};
	Color &operator = ( const Color& c )
	{
		this->ARGB = c.ARGB;
		return *this;
	}
	Color &operator = ( float c[4] )
	{
		memcpy( &ARGB, c, sizeof ARGB );
		return *this;
	}

	Color &operator = ( DWORD dwColor )
	{
		a = (( dwColor & 0xff000000 ) >> 24 ) / 255.0f;
		r = (( dwColor & 0xff0000 ) >> 16 ) / 255.0f;
		g = (( dwColor & 0xff00 ) >> 8 ) / 255.0f;
		b = (( dwColor & 0xff ) ) / 255.0f;
		return *this;
	}

	operator DWORD ()
	{
		DWORD dwRet;
		dwRet = (((( DWORD )( a * 255 ) ) & 0XFF ) << 24 ) | (((( DWORD )( r * 255 ) ) & 0XFF ) << 16 ) | (((( DWORD )( g * 255 ) ) & 0XFF ) << 8 ) | (((( DWORD )( b * 255 ) ) & 0XFF ) );
		return dwRet;
	}

	DWORD ToRGB()
	{
		DWORD R, G, B;
		float fR, fG, fB;
		fR = r * 255.0f + 256.0f;
		fG = g * 255.0f + 256.0f;
		fB = b * 255.0f + 256.0f;
		R  = (( *( int* ) & fR ) & 0x7f8000 ) << 1;
		G  = (( *( int* ) & fG ) & 0x7f8000 ) >> 7;
		B  = (( *( int* ) & fB ) & 0x7fffff ) >> 15;
		return R | G | B;
	}
};

/////////utili functions
MANIA_BASIS_API vector3 vec_normalize( const vector3& v );
MANIA_BASIS_API vector3 vec_cross( const vector3& vA, const vector3& vB );
MANIA_BASIS_API float vec_dot( const vector3& vA, const vector3& vB );
MANIA_BASIS_API vector3* vec_transform( vector3* v, const matrix44* mat );
MANIA_BASIS_API vector4* vec_transform( vector4* v, const matrix44* mat );
//MANIA_BASIS_API vector3* vec_rotat( vector3* v, const matrix44* mat );

MANIA_BASIS_API matrix44* build_matrix_perspective_fov_lh( matrix44* pOut,  float fovy,  float Aspect,  float zn,  float zf );
MANIA_BASIS_API matrix44* build_matrix_lookat_lh( matrix44* pOut, const vector3* pvEye, const vector3* pvLookAt, const vector3* pvUp );
MANIA_BASIS_API matrix44* build_matrix_rotation_axis( matrix44* pOut, e_axis_type AxisType, float fAngle );
MANIA_BASIS_API matrix44* build_matrix_trans_rot_scale( matrix44* pOut, const vector3& trans, const vector3& rot, const vector3& scale );

MANIA_BASIS_API float arctan2radian( float x, float y );
};
}