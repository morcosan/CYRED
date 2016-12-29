// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Console.h"
#include "CyredModule_Debug.h"

#include <iostream>
#include <cstdio>


using namespace CYRED;


Console::Console()
{
	// register console events
	EventManager::Singleton()->RegisterListener( EventType::CONSOLE_ERROR, this );
	EventManager::Singleton()->RegisterListener( EventType::CONSOLE_LOG, this );
}


Console::~Console()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( EventType::CONSOLE_ERROR, this );
	EventManager::Singleton()->UnregisterListener( EventType::CONSOLE_LOG, this );
}


void Console::OnEvent( EventType eType, void* eData )
{
	switch ( eType ) {
		case EventType::CONSOLE_LOG:
		case EventType::CONSOLE_ERROR:
		{
			DebugInfo* debugInfo = CAST_S( DebugInfo*, eData );
			std::cout << debugInfo->message << std::endl;
			break;
		}
	}
}
