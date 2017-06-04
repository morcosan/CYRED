// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#include "LauncherApp.h"


using namespace CYRED;


int main( int argc, char *argv[] )
{
	Application::CreateSingleton( LauncherApp::CreateSingleton() );
	CAST_S( LauncherApp*, Application::Singleton() )->Run( argc, argv );
	Application::DestroySingleton();

	return 0;
}