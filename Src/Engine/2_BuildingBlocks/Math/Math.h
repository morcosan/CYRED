// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix4.h"


namespace CYRED
{
	//! this class is a collection of functions for math 
	class DLL Math
	{
	public: 
		static const float INF;			// infinity
		static const float PI;			// the value of PI (180 degrees)
		static const float HALF_PI;		// the value of PI / 2 (90 degrees)


	private:
		Math() {}
		virtual ~Math() {}


	public:
		// returns the dot produc of 2 vectors
		static float	Dot			( const Vector3& firstVec, const Vector3& secondVec );
		
		// transforms each value of vector from radians to degrees
		static Vector3	ToDegrees	( const Vector3& vec );
		// transforms from radians to degrees
		static float	ToDegrees	( float value );

		// transforms each value of vector from degrees to radians
		static Vector3	ToRadians	( const Vector3& vec );
		// transforms from degrees to radians
		static float	ToRadians	( float value );

		// returns tangent of the given angle in radians
		static float	Tan			( float value );
		// returns cosinus of the given angle in radians
		static float	Cos			( float value );
		// returns sinus of the given angle in radians
		static float	Sin			( float value );

		// returns the max between the 2 values
		static float	Max			( float value1, float value2 );
		// returns the min between the 2 values
		static float	Min			( float value1, float value2 );
		
		// returns the value given, but no smaller than min or bigger than max
		static float	Clamp		( float value, float min, float max );
	};
}



