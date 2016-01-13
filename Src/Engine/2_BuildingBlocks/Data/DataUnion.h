// Copyright (c) 2015 Morco (www.morco.ro)
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
			STRING
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

		DataUnion& SetString		( const Char* value );
		DataUnion& SetFloat		( Float value );
		DataUnion& SetInt			( Int value );
		DataUnion& SetBool		( Bool value );
		DataUnion& SetVector2		( const Vector2& value );
		DataUnion& SetVector3		( const Vector3& value );
		DataUnion& SetVector4		( const Vector4& value );
		DataUnion& SetReference	( void* value );

		const Char*	GetString		()	const;
		Float			GetFloat		()	const;
		Int			GetInt			()	const;
		Bool			GetBool			()	const;
		Vector2		GetVector2		()	const;
		Vector3		GetVector3		()	const;
		Vector4		GetVector4		()	const;
		void*			GetReference	()	const;

		ValueType	GetValueType() const;


	protected:
		void*		_value;
		ValueType	_type;
	};
}