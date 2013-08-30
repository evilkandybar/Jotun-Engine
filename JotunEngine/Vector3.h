#pragma once

#include "stdafx.h"

class Vector3
{
      public:
           Vector3();
           Vector3( float, float, float );
		   Vector3( float* f ) {x = f[0]; y = f[1]; z = f[2];};

		   void set( float xDir, float yDir, float zDir ) {x = xDir; y = yDir; z = zDir;};
		   void set( Vector3 &other ) {*this = other;};

		   void setX( float xDir ) {x = xDir;};
		   void setY( float yDir ) {y = yDir;};
		   void setZ( float zDir ) {z = zDir;};

		   void normalize();
		   Vector3 normalized();
		   float magnitude();

		   const float getX() const {return x;};
		   const float getY() const {return y;};
		   const float getZ() const {return z;};

		   Vector3& operator=( const Vector3 &rhs );

		   bool operator==( const Vector3& );
		   bool operator!=( const Vector3& );

		   const Vector3& operator+( const Vector3 &rhs ) const {return Vector3( *this ) += rhs;};
		   const Vector3& operator-( const Vector3 &rhs ) const {return Vector3( *this ) -= rhs;};
		   const Vector3& operator*( const float &rhs ) const {return Vector3( *this ) *= rhs;};
		   const Vector3& operator/( const float &rhs ) const {return Vector3( *this ) /= rhs;};

		   Vector3& operator+=( const Vector3& );
		   Vector3& operator-=( const Vector3& );
		   Vector3& operator*=( const float& );
		   Vector3& operator/=( const float& );

		   float& operator[]( int index ) {if( index == 0 ) return x;
										  else if( index == 1 ) return y;
										  else if( index == 2 ) return z;};

		   float* toArray( float* );

           static float distance( Vector3&, Vector3& );
		   static float dot( Vector3&, Vector3& );
		   static Vector3 cross( Vector3&, Vector3& );

		   static Vector3 forwardVector( Vector3& );
		   static Vector3 upVector( Vector3& );
		   static Vector3 rightVector( Vector3& );

		   static Vector3 up;
		   static Vector3 right;
		   static Vector3 forward;
		   static Vector3 zero;
		   static Vector3 one;
      private:
           float x, y, z;
};