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
		FiniteString( cchar* format, ... );
		virtual ~FiniteString();

		bool	operator==	( cchar* other )	const;
		bool	operator!=	( cchar* other )	const;
		bool	operator<	( cchar* other )	const;


	public:
		void		Set			( cchar* format, ... );

		cchar*	GetChar		() const;
		int		GetLength	() const;


	private:
		char	_data[ MAX_SIZE ];
	};
}
