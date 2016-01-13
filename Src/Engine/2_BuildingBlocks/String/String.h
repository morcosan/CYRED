// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include <string>


namespace CYRED
{
	class DLL String
	{
	public:
		String();
		String( const Char* value );
		String( const String& value );
		virtual ~String();

		Bool	operator==	( const String& other )	const;
		Bool	operator!=	( const String& other )	const;
		void	operator=	( const String& other );
		Bool	operator<	( const String& other )	const;


	public:
		UInt		GetHash() const;
		const Char*	GetChar() const;


	protected:
		UInt			_hashValue;
		std::string*	_string;

	};
}


#define CUSTOM_STRING( buffer, pattern, ... )	sprintf( buffer, pattern, ##__VA_ARGS__ )
#define MAX_SIZE_CUSTOM_STRING	512