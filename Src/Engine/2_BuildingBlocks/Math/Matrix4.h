// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "glm/Include/fwd.hpp"


namespace CYRED
{
	struct Vector2;
	struct Vector3;
	struct Vector4;
}


namespace CYRED
{
	//! the class for matrix 4x4
	class DLL Matrix4
	{
	public:
		Matrix4();
		Matrix4( const Matrix4& other );
		Matrix4( const glm::mat4& other );
		virtual ~Matrix4();

		void	operator=( const Matrix4& other );

		Matrix4 operator-( const Matrix4& other );

		Matrix4 operator+( const Matrix4& other );

		Matrix4 operator/( const Matrix4& other );

		Matrix4 operator*( const Matrix4& other );
		Vector4 operator*( const Vector4& other );


	public:
		static Matrix4 Identity			();
		static Matrix4 Inverse			( const Matrix4& mat );
		static Matrix4 Translate			( const Matrix4& mat, const Vector3& other );
		static Matrix4 Scale				( const Matrix4& mat, const Vector3& other );
		static Matrix4 CreatePerspective	( float fovyAngle, float aspectRatio, float nearClipping, float farClipping );
		static Matrix4 CreateOrthogonal	( const Vector2& size, float nearClipping, float farClipping );


	public:
		const float* Ptr();


	protected:
		glm::mat4* _self;
	};
}