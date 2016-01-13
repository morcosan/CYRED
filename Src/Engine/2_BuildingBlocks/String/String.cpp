// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "String.h"


using namespace CYRED;



UInt HashFunction_Primes( const Char* s )
{
	const UInt a = 54059; /* a prime */
	const UInt b = 76963; /* another prime */
	const UInt c = 86969; /* yet another prime */

	UInt h = 31 /* also prime */;
	while ( *s ) 
	{
		h = (h * a) ^ (s[0] * b);
		s++;
	}
	return h % c; // return h or return h % C;
}



#define MAKE_HASH(s)	HashFunction_Primes(s)



String::String()
{
	_string = Memory::Alloc<std::string>( "" );
	_hashValue = MAKE_HASH( "" );
}


String::String( const Char* value )
{
	if ( value != NULL )
	{
		_string = Memory::Alloc<std::string>( value );
		_hashValue = MAKE_HASH( value );
	}
	else
	{
		_string = Memory::Alloc<std::string>( "" );
		_hashValue = MAKE_HASH( "" );
	}
}


String::String( const String& value )
{
	_string = Memory::Alloc<std::string>( *value._string );
	_hashValue = value._hashValue;
}


String::~String()
{
	Memory::Free( _string );
}


Bool String::operator==( const String& other ) const
{
	return (_hashValue == other.GetHash());
}


Bool String::operator!=( const String& other ) const
{
	return (_hashValue != other.GetHash());
}


void String::operator=( const String& other )
{
	_string = Memory::Alloc<std::string>( *other._string );
	_hashValue = other._hashValue;
}


Bool String::operator<( const String& other ) const
{
	return *_string < *other._string;
}


UInt String::GetHash() const
{
	return _hashValue;
}


const Char* String::GetChar() const
{
	return _string->c_str();
}
