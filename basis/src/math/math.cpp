#include "stdafx.h"
#include <math.h>
#include "Math.h"
#include <d3dx9.h>
namespace mania
{
namespace math
{
vector3 vec_normalize( const vector3& v )
{
	vector3 ret;
	float l = sqrtf( v.x * v.x + v.y * v.y + v.z * v.z );
	ret.x = v.x / l;
	ret.y = v.y / l;
	ret.z = v.z / l;
	return ret;
}

vector3 vec_cross( const vector3& vA, const vector3& vB )
{
	vector3 ret;
	ret.x = vA.y * vB.z - vA.z * vB.y;
	ret.y = vA.z * vB.x - vA.x * vB.z;
	ret.z = vA.x * vB.y - vA.y * vB.x;
	return ret;
}

float vec_dot( const vector3& vA, const vector3& vB )
{
	return vA.x * vB.x + vA.y * vB.y + vA.z * vB.z;
}

vector3* vec_transform( vector3* v, const matrix44* mat )
{
	float _x, _y, _z;
	_x = v->x * mat->m[0][0] + v->y * mat->m[1][0] + v->z * mat->m[2][0] + mat->m[3][0];
	_y = v->x * mat->m[0][1] + v->y * mat->m[1][1] + v->z * mat->m[2][1] + mat->m[3][1];
	_z = v->x * mat->m[0][2] + v->y * mat->m[1][2] + v->z * mat->m[2][2] + mat->m[3][2];
	v->x = _x;
	v->y = _y;
	v->z = _z;
	return v;
}

vector4* vec_transform( vector4* v, const matrix44* mat )
{
	float _x, _y, _z, _w;
	_x = v->x * mat->m[0][0] + v->y * mat->m[1][0] + v->z * mat->m[2][0] + v->w * mat->m[3][0];
	_y = v->x * mat->m[0][1] + v->y * mat->m[1][1] + v->z * mat->m[2][1] + v->w * mat->m[3][1];
	_z = v->x * mat->m[0][2] + v->y * mat->m[1][2] + v->z * mat->m[2][2] + v->w * mat->m[3][2];
	_w = v->x * mat->m[0][3] + v->y * mat->m[1][3] + v->z * mat->m[2][3] + v->w * mat->m[3][3];
	v->x = _x;
	v->y = _y;
	v->z = _z;
	v->w = _w;
	return v;
}

////matrix

matrix44* multiply_matrix( matrix44* out, const matrix44* m1, const matrix44* m2 )
{
	for ( int i = 0; i < 4; i ++ )
		for ( int j = 0; j < 4; j ++ )
		{
			out->m[i][j] = 0;
			for ( int k = 0; k < 4; k ++ )
			{
				out->m[i][j] += m1->m[i][k] * m2->m[k][j];
			}
		}

	out->m[0][0] =		m1->m[0][0]*m2->m[0][0] + 
						m1->m[0][1]*m2->m[1][0] + 
						m1->m[0][2]*m2->m[2][0] + 
						m1->m[0][3]*m2->m[3][0];
	out->m[0][1] =		m1->m[0][0]*m2->m[0][1] + 
						m1->m[0][1]*m2->m[1][1] + 
						m1->m[0][2]*m2->m[2][1] + 
						m1->m[0][3]*m2->m[3][1];
	out->m[0][2] =		m1->m[0][0]*m2->m[0][2] + 
						m1->m[0][1]*m2->m[1][2] + 
						m1->m[0][2]*m2->m[2][2] + 
						m1->m[0][3]*m2->m[3][2];
	out->m[0][3] =		m1->m[0][0]*m2->m[0][3] + 
						m1->m[0][1]*m2->m[1][3] + 
						m1->m[0][2]*m2->m[2][3] + 
						m1->m[0][3]*m2->m[3][3];

	out->m[1][0] =		m1->m[1][0]*m2->m[0][0] + 
						m1->m[1][1]*m2->m[1][0] + 
						m1->m[1][2]*m2->m[2][0] + 
						m1->m[1][3]*m2->m[3][0];
	out->m[1][1] =		m1->m[1][0]*m2->m[0][1] + 
						m1->m[1][1]*m2->m[1][1] + 
						m1->m[1][2]*m2->m[2][1] + 
						m1->m[1][3]*m2->m[3][1];
	out->m[1][2] =		m1->m[1][0]*m2->m[0][2] + 
						m1->m[1][1]*m2->m[1][2] + 
						m1->m[1][2]*m2->m[2][2] + 
						m1->m[1][3]*m2->m[3][2];
	out->m[1][3] =		m1->m[1][0]*m2->m[0][3] + 
						m1->m[1][1]*m2->m[1][3] + 
						m1->m[1][2]*m2->m[2][3] + 
						m1->m[1][3]*m2->m[3][3];

	out->m[2][0] =		m1->m[2][0]*m2->m[0][0] + 
						m1->m[2][1]*m2->m[1][0] + 
						m1->m[2][2]*m2->m[2][0] + 
						m1->m[2][3]*m2->m[3][0];
	out->m[2][1] =		m1->m[2][0]*m2->m[0][1] + 
						m1->m[2][1]*m2->m[1][1] + 
						m1->m[2][2]*m2->m[2][1] + 
						m1->m[2][3]*m2->m[3][1];
	out->m[2][2] =		m1->m[2][0]*m2->m[0][2] + 
						m1->m[2][1]*m2->m[1][2] + 
						m1->m[2][2]*m2->m[2][2] + 
						m1->m[2][3]*m2->m[3][2];
	out->m[2][3] =		m1->m[2][0]*m2->m[0][3] + 
						m1->m[2][1]*m2->m[1][3] + 
						m1->m[2][2]*m2->m[2][3] + 
						m1->m[2][3]*m2->m[3][3];

	out->m[3][0] =		m1->m[3][0]*m2->m[0][0] + 
						m1->m[3][1]*m2->m[1][0] + 
						m1->m[3][2]*m2->m[2][0] + 
						m1->m[3][3]*m2->m[3][0];
	out->m[3][1] =		m1->m[3][0]*m2->m[0][1] + 
						m1->m[3][1]*m2->m[1][1] + 
						m1->m[3][2]*m2->m[2][1] + 
						m1->m[3][3]*m2->m[3][1];
	out->m[3][2] =		m1->m[3][0]*m2->m[0][2] + 
						m1->m[3][1]*m2->m[1][2] + 
						m1->m[3][2]*m2->m[2][2] + 
						m1->m[3][3]*m2->m[3][2];
	out->m[3][3] =		m1->m[3][0]*m2->m[0][3] + 
						m1->m[3][1]*m2->m[1][3] + 
						m1->m[3][2]*m2->m[2][3] + 
						m1->m[3][3]*m2->m[3][3];

	return out;
}

matrix44* build_matrix_perspective_fov_lh( matrix44* pOut,  FLOAT fovy,  FLOAT Aspect,  FLOAT zn,  FLOAT zf )
{
	float yscale = 1.0f / tanf( fovy / 2.0f );
	float xscale = yscale / Aspect;
	pOut->m[0][0] = xscale;
	pOut->m[1][1] = yscale;
	pOut->m[2][2] = zf / ( zf - zn ) ;
	pOut->m[2][3] = 1;
	pOut->m[3][2] = -zn * zf / ( zf - zn );
	return pOut;
}

matrix44* build_matrix_lookat_lh( matrix44* pOut, const vector3* pvEye, const vector3* pvLookAt, const vector3* pvUp )
{
	vector3 ZAxis, YAxis, XAxis;
	ZAxis = ( *pvLookAt - *pvEye );
	ZAxis.normalize();
	XAxis = vec_cross( *pvUp, ZAxis );
	XAxis.normalize();
	YAxis = vec_cross( ZAxis, XAxis );
	YAxis.normalize();
	pOut->m[0][0] = XAxis.x;
	pOut->m[0][1] = YAxis.x;
	pOut->m[0][2] = ZAxis.x;
	pOut->m[1][0] = XAxis.y;
	pOut->m[1][1] = YAxis.y;
	pOut->m[1][2] = ZAxis.y;
	pOut->m[2][0] = XAxis.z;
	pOut->m[2][1] = YAxis.z;
	pOut->m[2][2] = ZAxis.z;
	pOut->m[3][0] = -vec_dot( XAxis, *pvEye );
	pOut->m[3][1] = -vec_dot( YAxis, *pvEye );
	pOut->m[3][2] = -vec_dot( ZAxis, *pvEye );
	pOut->m[0][3] = 0;
	pOut->m[1][3] = 0;
	pOut->m[2][3] = 0;
	pOut->m[3][3] = 1;
	return pOut;
}

matrix44* build_matrix_rotation_axis( matrix44* pOut, e_axis_type AxisType, float radian )
{
	pOut->set_identity();
	switch ( AxisType )
	{
	case axis_x:
		pOut->m[1][1] = cos( radian );
		pOut->m[1][2] = sin( radian );
		pOut->m[2][1] = -sin( radian );
		pOut->m[2][2] = cos( radian );
		break;
	case axis_y:
		pOut->m[0][0] = cos( radian );
		pOut->m[0][2] = -sin( radian );
		pOut->m[2][0] = sin( radian );
		pOut->m[2][2] = cos( radian );
		break;
	case axis_z:
		pOut->m[0][0] = cos( radian );
		pOut->m[0][1] = sin( radian );
		pOut->m[1][0] = -sin( radian );
		pOut->m[1][1] = cos( radian );
		break;
	}
	return pOut;
}

matrix44* build_matrix_trans_rot_scale( matrix44* pOut, const vector3& trans, const vector3& rot/*yaw pitch roll*/, const vector3& scale )
{
	//rotation
	float cx = cos( rot.x );
	float cy = cos( rot.y );
	float cz = cos( rot.z );
	float sx = sin( rot.x );
	float sy = sin( rot.y );
	float sz = sin( rot.z );
	*pOut = matrix44(	cy*cz + (-sy)*(-sx)*(-sz),	cy*sz + (-sy)*(-sx)*cz,		(-sy)*cx,	0,
						cx*(-sz),					cx * cz,					sx,			0,
						sy*cz + cy*(-sx)*(-sz),		sy*sz + cy*(-sx)*cz,		cy*cx,		0,
						trans.x,					trans.y,					trans.z,	1);
	return pOut;
}

void inverse_matrix( matrix44* m )
{
	D3DXMATRIX d3dm = *(D3DXMATRIX*)m;
	D3DXMATRIX dm;
	::D3DXMatrixIdentity(&dm);
	dm._41 = 41;
	dm._42 = 42;
	dm._43 = 43;
	::D3DXMatrixInverse( &dm, NULL, &dm );
	::D3DXMatrixInverse( (D3DXMATRIX*)m, NULL, &d3dm );

}

float arctan2radian( float x, float y ) 
{
	return atan2f( x, y );
}
}
}