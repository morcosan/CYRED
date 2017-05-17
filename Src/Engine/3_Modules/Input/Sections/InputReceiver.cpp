// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "InputReceiver.h"
#include "InputEvent.h"


using namespace CYRED;


void InputReceiver::ClearQueue()
{
	_eventQueue.Clear();
}


int InputReceiver::GetEventsCount()
{
	return _eventQueue.Size();
}


InputEvent& CYRED::InputReceiver::GetEventAt( int index )
{
	ASSERT( index < _eventQueue.Size() );

	return _eventQueue[ index ];
}
