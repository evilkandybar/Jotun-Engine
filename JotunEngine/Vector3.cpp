#include "stdafx.h"
#include "Vector3.h"

Vector3 Vector3::right = Vector3( 1, 0, 0 );
Vector3 Vector3::up = Vector3( 0, 1, 0 );
Vector3 Vector3::forward = Vector3( 0, 0, 1 );
Vector3 Vector3::zero = Vector3( 0, 0, 0 );
Vector3 Vector3::one = Vector3( 1, 1, 1 );

Vector3::Vector3()
{
     x = 0;
     y = 0;
     z = 0;
}

Vector3::Vector3( float a, float b, float c )
{
     x = a;
     y = b;
     z = c;
}

Vector3& Vector3::operator=(const Vector3 &rhs)
 {
	 if( this != &rhs )
	 {
		 x = rhs.getX();
		 y = rhs.getY();
		 z = rhs.getZ();
	 }
	 return *this;
 }

bool Vector3::operator==( const Vector3 &lhs )
{
	return lhs.getX() == x && lhs.getY() == y && lhs.getZ() == z;
}

bool Vector3::operator!=( const Vector3 &lhs )
{
	return !(*this == lhs);
}

Vector3& Vector3::operator+=( const Vector3 &rhs )
{
	x += rhs.getX();
	y += rhs.getY();
	z += rhs.getZ();
	return *this;
}

Vector3& Vector3::operator-=( const Vector3 &rhs )
{
	x -= rhs.getX();
	y -= rhs.getY();
	z -= rhs.getZ();
	return *this;
}

Vector3& Vector3::operator*=( const float &rhs )
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}

Vector3& Vector3::operator/=( const float &rhs )
{
	x /= rhs;
	y /= rhs;
	z /= rhs;
	return *this;
}

float* Vector3::toArray( float* ar )
{
	ar = new float[3];
	ar[0] = x;
	ar[2] = z;
	ar[1] = y;
	return ar;
}

void Vector3::normalize()
{
	float m = magnitude();
	x /= m;
	y /= m;
	z /= m;
}

Vector3 Vector3::normalized()
{
	float m = magnitude();
	return Vector3( x/m, y/m, z/m );
}

float Vector3::magnitude()
{
	return sqrt((x*x)+(y*y)+(z*z));
}

float Vector3::distance( Vector3 &pos1, Vector3 &pos2 )
{
	float x = pos1.getX()-pos2.getX();
	float y = pos1.getY()-pos2.getY();
	float z = pos1.getZ()-pos2.getZ();
	return sqrt((x*x)+(y*y)+(z*z));
}

float Vector3::dot( Vector3 &rhs, Vector3 &lhs )
{
	float value = 0;
	for( int i = 0; i < 3; i++ ) {
		value += rhs[i] * lhs[i];
	}
	return value;
}

Vector3 Vector3::cross( Vector3 &lhs, Vector3 &rhs )
{
	Vector3 value;
	value.setX( lhs.getY()*rhs.getZ() - lhs.getZ()*rhs.getY() );
	value.setY( lhs.getZ()*rhs.getX() - lhs.getX()*rhs.getZ() );
	value.setZ( lhs.getX()*rhs.getY() - lhs.getY()*rhs.getX() );
	return value;
}

Vector3 Vector3::forwardVector( Vector3 &angles )
{
	float theta, sx, sy, sz, cx, cy, cz;
	Vector3 forward;

	// rotation angle about X-axis (pitch)
    theta = angles[0] * DEG2RAD;
    sx = sinf(theta);
    cx = cosf(theta);

    // rotation angle about Y-axis (yaw)
    theta = angles[1] * DEG2RAD;
    sy = sinf(theta);
    cy = cosf(theta);

    // rotation angle about Z-axis (roll)
    theta = angles[2] * DEG2RAD;
    sz = sinf(theta);
    cz = cosf(theta);

	// determine forward vector
    forward.setX( sy*cz + cy*sx*sz );
    forward.setY( sy*sz - cy*sx*cz );
    forward.setZ( cy*cx );

	return forward;
}

Vector3 Vector3::upVector( Vector3 &angles )
{
	float theta, sx, sy, sz, cx, cy, cz;
	Vector3 up;

	// rotation angle about X-axis (pitch)
    theta = angles[0] * DEG2RAD;
    sx = sinf(theta);
    cx = cosf(theta);

    // rotation angle about Y-axis (yaw)
    theta = angles[1] * DEG2RAD;
    sy = sinf(theta);
    cy = cosf(theta);

    // rotation angle about Z-axis (roll)
    theta = angles[2] * DEG2RAD;
    sz = sinf(theta);
    cz = cosf(theta);

	// determine up vector
    up.setX( -cx*sz );
    up.setY( cx*cz );
    up.setZ( sx );

	return up;
}

Vector3 Vector3::rightVector( Vector3 &angles )
{
	float theta, sx, sy, sz, cx, cy, cz;
	Vector3 right;

	// rotation angle about X-axis (pitch)
    theta = angles[0] * DEG2RAD;
    sx = sinf(theta);
    cx = cosf(theta);

    // rotation angle about Y-axis (yaw)
    theta = angles[1] * DEG2RAD;
    sy = sinf(theta);
    cy = cosf(theta);

    // rotation angle about Z-axis (roll)
    theta = angles[2] * DEG2RAD;
    sz = sinf(theta);
    cz = cosf(theta);

	right.setX( cy*cz - sy*sx*sz );
    right.setY( cy*sz + sy*sx*cz );
    right.setZ( -sy*cx );

	return right;
}