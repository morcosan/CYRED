// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "glm/Include/fwd.hpp"


namespace CYRED
{
	struct DLL Vector3
	{
		float x;
		float y;
		float z;

		Vector3();
		Vector3( const Vector3& other );
		Vector3( float xx, float yy, float zz );

		void	operator=	( const Vector3& other );
		void	operator+=	( const Vector3& other );
		void	operator-=	( const Vector3& other );
		void	operator/=	( const Vector3& other );

		void	operator*=	( const Vector3& other );
		void	operator*=	( float other );
		void	operator*=	( int other );

		bool	operator==	( const Vector3& other )	const;
		bool	operator!=	( const Vector3& other )	const;

		Vector3 operator-	( const Vector3& other )	const;
		Vector3 operator-	()							const;

		Vector3 operator+	( const Vector3& other )	const;

		Vector3 operator/	( const Vector3& other )	const;
		Vector3 operator/	( float other )				const;
		Vector3 operator/	( int other )				const;

		Vector3 operator*	( const Vector3& other )	const;
		Vector3 operator*	( float other )				const;
		Vector3 operator*	( int other )				const;

		const float* Ptr();
	};
}