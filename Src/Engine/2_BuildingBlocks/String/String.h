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
		String( cchar* other );
		String( const String& other );
		virtual ~String();

		void	operator=	( const String& other );
		void	operator=	( cchar* other );
		bool	operator==	( const String& other )	const;
		bool	operator==	( cchar* other )	const;
		bool	operator!=	( const String& other )	const;
		bool	operator!=	( cchar* other )	const;
		bool	operator<	( const String& other )	const;
		bool	operator<	( cchar* other )	const;


	public:
		cchar*	GetChar		() const;
		int		GetLength	() const;


	private:
		char*	_data;
	};
}
