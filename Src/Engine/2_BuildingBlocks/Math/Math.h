// Copyright (c) 2015 Morco (www.morco.ro)
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
		static const Float PI;			//! the value of PI (180 degrees)
		static const Float HALF_PI;		//! the value of PI / 2 (90 degrees)


	private:
		Math() {}
		virtual ~Math() {}


	public:
		static	//! returns the dot produc of 2 vectors
		Float		Dot			( const Vector3& firstVec, const Vector3& secondVec );
		
		static	//! transforms each value of vector from radians to degrees
		Vector3	ToDegrees	( const Vector3& vec );
		static	//! transforms from radians to degrees
		Float		ToDegrees	( Float value );

		static	//! transforms each value of vector from degrees to radians
		Vector3	ToRadians	( const Vector3& vec );
		static	//! transforms from degrees to radians
		Float		ToRadians	( Float value );

		static	//! returns tangent of the given angle in radians
		Float		Tan			( Float value );
		static	//! returns cosinus of the given angle in radians
		Float		Cos			( Float value );
		static	//! returns sinus of the given angle in radians
		Float		Sin			( Float value );

		static	//! returns the max between the 2 values
		Float		Max			( Float value1, Float value2 );
		static	//! returns the min between the 2 values
		Float		Min			( Float value1, Float value2 );
		
		static	//! returns the value given, but no smaller than min or bigger than max
		Float		Clamp		( Float value, Float min, Float max );
	};
}



