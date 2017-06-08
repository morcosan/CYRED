// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "String.h"

#include <cstring>


using namespace CYRED;



String::String()
	: _data( new char[1] )
{
	_data[0] = '\0';
}


String::String( cchar* other )
{
	if ( other == NULL )
	{
		_data = new char[1];
		_data[0] = '\0';
		return;
	}

	_data = new char[ strlen( other ) + 1 ];
	strcpy( _data, other );
}


String::String( const String& other )
	: _data( new char[ other.GetLength() + 1 ] )
{
	strcpy( _data, other.GetChar() );
}


String::~String()
{
	ARRAY_FREE( _data );
}


void String::operator=( const String& other )
{
	if ( this == &other )
	{
		return;
	}

	ARRAY_FREE( _data );
	_data = new char[ other.GetLength() + 1 ];
	strcpy( _data, other.GetChar() );
}


void String::operator=( cchar* other )
{
	if ( other == NULL )
	{
		_data = new char[1];
		_data[0] = '\0';
		return;
	}

	ARRAY_FREE( _data );
	_data = new char[ strlen( other ) + 1 ];
	strcpy( _data, other );
}


bool String::operator==( const String& other ) const
{
	return ( strcmp( _data, other.GetChar() ) == 0 );
}


bool String::operator==( cchar* other ) const
{
	if ( other == NULL )
	{
		return ( GetLength() == 0 );
	}

	return ( strcmp( _data, other ) == 0 );
}


bool String::operator!=( const String& other ) const
{
	return !operator==( other );
}


bool String::operator!=( cchar* other ) const
{
	return !operator==( other );
}


bool String::operator<( const String& other ) const
{
	return ( strcmp( _data, other.GetChar() ) < 0 );
}


bool String::operator<( cchar* other ) const
{
	if ( other == NULL )
	{
		return FALSE;
	}

	return ( strcmp( _data, other ) < 0 );
}


cchar* String::GetChar() const
{
	return _data;
}


int String::GetLength() const
{
	return strlen( _data );
}


int String::Length( cchar* other )
{
	return strlen( other );
}
