// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "CyredModule_Debug.h"


namespace CYRED
{
	class ConsoleWindows : public Console
	{
	public:
		ConsoleWindows() {}
		virtual ~ConsoleWindows() {}


	public:
		void Log( const Char* text )  override;
	};
}