// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"


namespace CYRED
{
	class DLL FiniteString
	{
	public:
		static const int MAX_SIZE = 512;


	public:
		FiniteString();
		FiniteString( const char* format, ... );
		virtual ~FiniteString();

		bool	operator==	( const char* other )	const;
		bool	operator!=	( const char* other )	const;
		bool	operator<	( const char* other )	const;


	public:
		void		Set			( const char* format, ... );

		const char*	GetChar		() const;
		int		GetLength	() const;


	protected:
		char	_data[ MAX_SIZE ];
	};
}
