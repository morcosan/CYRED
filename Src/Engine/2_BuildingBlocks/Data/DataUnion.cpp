// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "DataUnion.h"
#include "../String/String.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"


using namespace CYRED;


DataUnion::DataUnion() 
	: _value( NULL ) 
	, _type( NONE )
{
}


DataUnion::DataUnion( const DataUnion& other )
	: _value( NULL ) 
	, _type( NONE )
{
	switch ( other._type )
	{
	case STRING:
		SetString( other.GetString() );
		break;

	case FLOAT:
		SetFloat( other.GetFloat() );
		break;

	case INT:
		SetInt( other.GetInt() );
		break;

	case BOOL:
		SetBool( other.GetBool() );
		break;

	case VECTOR2:
		SetVector2( other.GetVector2() );
		break;

	case VECTOR3:
		SetVector3( other.GetVector3() );
		break;

	case VECTOR4:
		SetVector4( other.GetVector4() );
		break;

	case REFERENCE:
		SetReference( other.GetReference() );
		break;
	}
}


DataUnion::~DataUnion()
{
	if ( _type != REFERENCE )
	{
		Memory::Free( _value );
	}
}


void DataUnion::operator=( const DataUnion& other )
{
	switch ( other._type )
	{
		case STRING:
			SetString( other.GetString() );
			break;

		case FLOAT:
			SetFloat( other.GetFloat() );
			break;

		case INT:
			SetInt( other.GetInt() );
			break;

		case BOOL:
			SetBool( other.GetBool() );
			break;

		case VECTOR2:
			SetVector2( other.GetVector2() );
			break;

		case VECTOR3:
			SetVector3( other.GetVector3() );
			break;

		case VECTOR4:
			SetVector4( other.GetVector4() );
			break;

		case REFERENCE:
			SetReference( other.GetReference() );
			break;
	}
}


DataUnion& DataUnion::SetString( const Char* value )
{
	if ( _type != REFERENCE )
	{
		Memory::Free( _value );
	}
	_value = Memory::Alloc<String>( value );
	_type = ValueType::STRING;

	return *this;
}


DataUnion& DataUnion::SetFloat( Float value )
{
	if ( _type != REFERENCE )
	{
		Memory::Free( _value );
	}
	_value = Memory::Alloc<Float>( value );
	_type = ValueType::FLOAT;

	return *this;
}


DataUnion& DataUnion::SetInt( Int value )
{
	if ( _type != REFERENCE )
	{
		Memory::Free( _value );
	}
	_value = Memory::Alloc<Int>( value );
	_type = ValueType::INT;

	return *this;
}


DataUnion& DataUnion::SetBool( Bool value )
{
	if ( _type != REFERENCE )
	{
		Memory::Free( _value );
	}
	_value = Memory::Alloc<Bool>( value );
	_type = ValueType::BOOL;

	return *this;
}


DataUnion& DataUnion::SetVector2( const Vector2& value )
{
	if ( _type != REFERENCE )
	{
		Memory::Free( _value );
	}
	_value = Memory::Alloc<Vector2>( value );
	_type = ValueType::VECTOR2;

	return *this;
}


DataUnion& DataUnion::SetVector3( const Vector3& value )
{
	if ( _type != REFERENCE )
	{
		Memory::Free( _value );
	}
	_value = Memory::Alloc<Vector3>( value );
	_type = ValueType::VECTOR3;

	return *this;
}


DataUnion& DataUnion::SetVector4( const Vector4& value )
{
	if ( _type != REFERENCE )
	{
		Memory::Free( _value );
	}
	_value = Memory::Alloc<Vector4>( value );
	_type = ValueType::VECTOR4;

	return *this;
}


DataUnion& DataUnion::SetReference( void* value )
{
	if ( _type != REFERENCE )
	{
		Memory::Free( _value );
	}
	_value = value;
	_type = ValueType::REFERENCE;

	return *this;
}


const Char* DataUnion::GetString() const
{
	ASSERT( _value != NULL );
	ASSERT( _type == ValueType::STRING );

	return CAST_S( String*, _value )->GetChar();
}


Float DataUnion::GetFloat() const
{
	ASSERT( _value != NULL );
	ASSERT( _type == ValueType::FLOAT );

	return *CAST_S( Float*, _value );
}


Int DataUnion::GetInt() const
{
	ASSERT( _value != NULL );
	ASSERT( _type == ValueType::INT );

	return *CAST_S( Int*, _value );
}


Bool DataUnion::GetBool() const
{
	ASSERT( _value != NULL );
	ASSERT( _type == ValueType::BOOL );

	return *CAST_S( Bool*, _value );
}


Vector2 DataUnion::GetVector2() const
{
	ASSERT( _value != NULL );
	ASSERT( _type == ValueType::VECTOR2 );

	return *CAST_S( Vector2*, _value );
}


Vector3 DataUnion::GetVector3() const
{
	ASSERT( _value != NULL );
	ASSERT( _type == ValueType::VECTOR3 );

	return *CAST_S( Vector3*, _value );
}


Vector4 DataUnion::GetVector4() const
{
	ASSERT( _value != NULL );
	ASSERT( _type == ValueType::VECTOR4 );

	return *CAST_S( Vector4*, _value );
}


void* DataUnion::GetReference() const
{
	ASSERT( _type == ValueType::REFERENCE );

	return _value;
}


DataUnion::ValueType DataUnion::GetValueType() const
{
	return _type;
}
