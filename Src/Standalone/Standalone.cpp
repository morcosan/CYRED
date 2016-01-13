// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Standalone.h"
#include "App/StandaloneApp.h"
#include "GameController.h"


using namespace CYRED;
using namespace NotAPI;


Standalone::Standalone( Int &argc, Char* argv[], GameController* gameController )
{
	Application::CreateSingleton( Memory::Alloc<StandaloneApp>() );
	CAST_S( StandaloneApp*, Application::Singleton() )->Run( argc, argv, gameController );
}


Standalone::Standalone( Int &argc, Char* argv[], Char* configFilePath )
{
	Application::CreateSingleton( Memory::Alloc<StandaloneApp>() );
	CAST_S( StandaloneApp*, Application::Singleton() )->Run( argc, argv, configFilePath );
}


Standalone::~Standalone()
{
	Application::DestroySingleton();
}

