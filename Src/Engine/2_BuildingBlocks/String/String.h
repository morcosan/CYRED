// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"


namespace CYRED
{
	class DLL String
	{
	public:
		String();
		String( const Char* other );
		String( const String& other );
		virtual ~String();

		void	operator=	( const String& other );
		void	operator=	( const Char* other );
		Bool	operator==	( const String& other )	const;
		Bool	operator==	( const Char* other )	const;
		Bool	operator!=	( const String& other )	const;
		Bool	operator!=	( const Char* other )	const;
		Bool	operator<	( const String& other )	const;
		Bool	operator<	( const Char* other )	const;


	public:
		const Char*	GetChar		() const;
		UInt		GetLength	() const;


	protected:
		Char*	_data;
	};
}
