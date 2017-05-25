// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"


namespace CYRED
{
	struct Vector2;
	struct Vector3;
	struct Vector4;
}


namespace CYRED
{
	class DLL DataUnion
	{
	public:
		enum ValueType
		{
			NONE
			, STRING
			, FLOAT
			, INT
			, BOOL
			, VECTOR2
			, VECTOR3
			, VECTOR4
			, REFERENCE
		};


	public:
		DataUnion();
		DataUnion( const DataUnion& other );
		virtual ~DataUnion();

		void operator=( const DataUnion& other );

		DataUnion& SetString	( cchar* value );
		DataUnion& SetFloat		( float value );
		DataUnion& SetInt		( int value );
		DataUnion& SetBool		( bool value );
		DataUnion& SetVector2	( const Vector2& value );
		DataUnion& SetVector3	( const Vector3& value );
		DataUnion& SetVector4	( const Vector4& value );
		DataUnion& SetReference	( void* value );

		cchar*	GetString		()	const;
		float		GetFloat		()	const;
		int			GetInt			()	const;
		bool		GetBool			()	const;
		Vector2		GetVector2		()	const;
		Vector3		GetVector3		()	const;
		Vector4		GetVector4		()	const;
		void*		GetReference	()	const;

		ValueType	GetValueType() const;


	private:
		void*		_value;
		ValueType	_type;
	};
}