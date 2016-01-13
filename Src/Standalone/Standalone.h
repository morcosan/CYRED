// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"


namespace CYRED
{
	class GameController;

	class DLL Standalone
	{
	public:
		// for C++ only apps
		Standalone( Int& argc, Char* argv[], GameController* gameController );

		// start from an external configuration file
		Standalone( Int& argc, Char* argv[], Char* configFilePath );

		virtual ~Standalone();
	};
}
