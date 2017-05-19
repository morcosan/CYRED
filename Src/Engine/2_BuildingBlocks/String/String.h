// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"


namespace CYRED
{
	class DLL String
	{
	public:
		String();
		String( const char* other );
		String( const String& other );
		virtual ~String();

		void	operator=	( const String& other );
		void	operator=	( const char* other );
		bool	operator==	( const String& other )	const;
		bool	operator==	( const char* other )	const;
		bool	operator!=	( const String& other )	const;
		bool	operator!=	( const char* other )	const;
		bool	operator<	( const String& other )	const;
		bool	operator<	( const char* other )	const;


	public:
		const char*	GetChar		() const;
		int		GetLength	() const;


	private:
		char*	_data;
	};
}
