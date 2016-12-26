// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "glm/Include/fwd.hpp"


namespace CYRED
{
	struct DLL Vector4
	{
		Float x;
		Float y;
		Float z;
		Float w;

		Vector4();
		Vector4( const Vector4& other );
		Vector4( Float xx, Float yy, Float zz, Float ww );

		void	operator=( const Vector4& other );

		Bool	operator==( const Vector4& other )	const;
		Bool	operator!=( const Vector4& other )	const;

		Vector4 operator-( const Vector4& other )	const;

		Vector4 operator+( const Vector4& other )	const;

		Vector4 operator/( const Vector4& other )	const;
		Vector4 operator/( Float other )			const;
		Vector4 operator/( UInt other )				const;
		Vector4 operator/( Int other )				const;

		Vector4 operator*( const Vector4& other )	const;
		Vector4 operator*( Float other )			const;	
		Vector4 operator*( UInt other )				const;
		Vector4 operator*( Int other )				const;

		const Float* Ptr();


	// scripting
	public:
		Float	S_GetX() const;
		Float	S_GetY() const;
		Float	S_GetZ() const;
		Float	S_GetW() const;
		void	S_SetX( float value );
		void	S_SetY( float value );
		void	S_SetZ( float value );
		void	S_SetW( float value );
	};
}