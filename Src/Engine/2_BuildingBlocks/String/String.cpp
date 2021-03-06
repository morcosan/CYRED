// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "String.h"

#include <cstring>


using namespace CYRED;



String::String()
	: _data( Memory::AllocArray<Char>(1) )
{
	_data[0] = '\0';
}


String::String( const Char* other )
{
	if ( other == NULL )
	{
		_data = Memory::AllocArray<Char>( 1 );
		_data[0] = '\0';
		return;
	}

	_data = Memory::AllocArray<Char>( strlen( other ) + 1 );
	strcpy( _data, other );
}


String::String( const String& other )
	: _data( Memory::AllocArray<Char>( other.GetLength() + 1 ) )
{
	strcpy( _data, other.GetChar() );
}


String::~String()
{
	Memory::FreeArray( _data );
}


void String::operator=( const String& other )
{
	if ( this == &other )
	{
		return;
	}

	Memory::FreeArray( _data );
	_data = Memory::AllocArray<Char>( other.GetLength() + 1 );
	strcpy( _data, other.GetChar() );
}


void String::operator=( const Char* other )
{
	if ( other == NULL )
	{
		_data = Memory::AllocArray<Char>( 1 );
		_data[0] = '\0';
		return;
	}

	Memory::FreeArray( _data );
	_data = Memory::AllocArray<Char>( strlen( other ) + 1 );
	strcpy( _data, other );
}


Bool String::operator==( const String& other ) const
{
	return ( strcmp( _data, other.GetChar() ) == 0 );
}


Bool String::operator==( const Char* other ) const
{
	if ( other == NULL )
	{
		return ( GetLength() == 0 );
	}

	return ( strcmp( _data, other ) == 0 );
}


Bool String::operator!=( const String& other ) const
{
	return !operator==( other );
}


Bool String::operator!=( const Char* other ) const
{
	return !operator==( other );
}


Bool String::operator<( const String& other ) const
{
	return ( strcmp( _data, other.GetChar() ) < 0 );
}


Bool String::operator<( const Char* other ) const
{
	if ( other == NULL )
	{
		return FALSE;
	}

	return ( strcmp( _data, other ) < 0 );
}


const Char* String::GetChar() const
{
	return _data;
}


UInt String::GetLength() const
{
	return strlen( _data );
}
