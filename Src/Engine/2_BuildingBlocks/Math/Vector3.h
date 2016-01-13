// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "glm/Include/fwd.hpp"


namespace CYRED
{
	struct DLL Vector3
	{
		Float x;
		Float y;
		Float z;

		Vector3();
		Vector3( const Vector3& other );
		Vector3( Float xx, Float yy, Float zz );

		void	operator=	( const Vector3& other );
		void	operator+=	( const Vector3& other );
		void	operator-=	( const Vector3& other );
		void	operator/=	( const Vector3& other );

		void	operator*=	( const Vector3& other );
		void	operator*=	( Float other );
		void	operator*=	( UInt other );
		void	operator*=	( Int other );

		Bool	operator==	( const Vector3& other )	const;
		Bool	operator!=	( const Vector3& other )	const;

		Vector3 operator-	( const Vector3& other )	const;
		Vector3 operator-	()							const;

		Vector3 operator+	( const Vector3& other )	const;

		Vector3 operator/	( const Vector3& other )	const;
		Vector3 operator/	( Float other )				const;
		Vector3 operator/	( UInt other )				const;
		Vector3 operator/	( Int other )				const;

		Vector3 operator*	( const Vector3& other )	const;
		Vector3 operator*	( Float other )				const;
		Vector3 operator*	( UInt other )				const;
		Vector3 operator*	( Int other )				const;

		const Float* Ptr();
	};
}