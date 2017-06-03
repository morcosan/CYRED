// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "glm/Include/fwd.hpp"


namespace CYRED
{
	struct Vector3;
	class Matrix4;


	class DLL Quaternion
	{
	public:
		Quaternion();
		Quaternion( const Quaternion& other );
		Quaternion( const glm::quat& other );
		Quaternion( const Vector3& other );
		Quaternion( float x, float y, float z, float w );
		virtual ~Quaternion();

		void		operator=( const Quaternion& other );
		Quaternion	operator+( const Quaternion& other ) const;
		Quaternion	operator*( const Quaternion& other ) const;


	public:
		static Quaternion	Inverse		( const Quaternion& other );
		static Quaternion	Normalize	( const Quaternion& other );
		static Vector3		EulerAngles	( const Quaternion& other );


	public:
		Vector3 ToEulerAngles	()							const;
		Matrix4 ToMatrix4		()							const;
		Vector3 ApplyRotation	( const Vector3& other );


	private:
		glm::quat* _self;
	};
}