// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"

namespace CYRED
{
	struct Vector3;
	class Matrix4;
}


namespace CYRED
{
	struct DLL Quaternion
	{
		float x;
		float y;
		float z;
		float w;

		Quaternion();
		Quaternion( const Quaternion& other );
		Quaternion( const Vector3& other );
		Quaternion( float xx, float yy, float zz, float ww );
		virtual ~Quaternion();

		void		operator=( const Quaternion& other );
		Quaternion	operator+( const Quaternion& other ) const;
		Quaternion	operator*( const Quaternion& other ) const;

		Vector3 ToEulerAngles	()	const;
		Matrix4 ToMatrix4		()	const;
		Vector3 ApplyRotation	( const Vector3& other );

		static Quaternion	Inverse		( const Quaternion& other );
		static Quaternion	Normalize	( const Quaternion& other );
		static Vector3		EulerAngles	( const Quaternion& other );
	};
}