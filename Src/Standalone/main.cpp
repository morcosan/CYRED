// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#include "StandaloneApp.h"


using namespace CYRED;


int main( int argc, char *argv[] )
{
	Application::CreateSingleton( StandaloneApp::CreateSingleton() );
	CAST_S( StandaloneApp*, Application::Singleton() )->Run( argc, argv );
	Application::DestroySingleton();

	return 0;
}