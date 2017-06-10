// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Console.h"
#include "CyredModule_Debug.h"

#include <iostream>
#include <cstdio>


using namespace CYRED;


Console::Console()
{
	// register console events
	EventManager::Singleton()->Register( EventType::CONSOLE_ERROR, this );
	EventManager::Singleton()->Register( EventType::CONSOLE_LOG, this );
}


Console::~Console()
{
	// unregister events
	EventManager::Singleton()->Unregister( EventType::CONSOLE_ERROR, this );
	EventManager::Singleton()->Unregister( EventType::CONSOLE_LOG, this );
}


void Console::OnEvent( int eventType, void* eventData )
{
	switch ( eventType ) {
		case EventType::CONSOLE_LOG:
		case EventType::CONSOLE_ERROR:
		{
			DebugInfo* debugInfo = CAST_S( DebugInfo*, eventData );
			std::cout << debugInfo->message << std::endl;
			break;
		}
	}
}
