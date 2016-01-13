// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"


namespace CYRED
{
	ABSTRACT class DLL Console
	{
	public:
		Console() {}
		virtual ~Console() {}


	public:
		virtual void Log( const Char* text )  PURE_VIRTUAL;
	};
}