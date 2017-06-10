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
	EventManager::Singleton()->Register( this, EventType::CONSOLE_ERROR, EventListenMode::SYNC );
	EventManager::Singleton()->Register( this, EventType::CONSOLE_LOG,	 EventListenMode::SYNC );
}


Console::~Console()
{
	// unregister events
	EventManager::Singleton()->Unregister( this, EventType::CONSOLE_ERROR,  EventListenMode::SYNC );
	EventManager::Singleton()->Unregister( this, EventType::CONSOLE_LOG,	EventListenMode::SYNC );
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
