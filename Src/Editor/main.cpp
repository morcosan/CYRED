// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#include "EditorApp.h"


using namespace CYRED;


int main( int argc, char *argv[] )
{
	Application::CreateSingleton( EditorApp::CreateSingleton() );
	CAST_S( EditorApp*, Application::Singleton() )->Run( argc, argv );
	Application::DestroySingleton();

	return 0;
}