// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"


namespace CYRED
{
	class DLL FiniteString
	{
	public:
		static const UInt MAX_SIZE = 512;


	public:
		FiniteString();
		FiniteString( const Char* format, ... );
		virtual ~FiniteString();

		Bool	operator==	( const Char* other )	const;
		Bool	operator!=	( const Char* other )	const;
		Bool	operator<	( const Char* other )	const;


	public:
		void		Set			( const Char* format, ... );

		const Char*	GetChar		() const;
		UInt		GetLength	() const;


	protected:
		Char	_data[ MAX_SIZE ];
	};
}
