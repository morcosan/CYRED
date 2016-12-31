// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "InputReceiver.h"
#include "InputEvent.h"


using namespace CYRED;


void InputReceiver::ClearQueue()
{
	_eventQueue.Clear();
}


UInt InputReceiver::GetEventsCount()
{
	return _eventQueue.Size();
}


InputEvent& CYRED::InputReceiver::GetEventAt( UInt index )
{
	ASSERT( index < _eventQueue.Size() );

	return _eventQueue[ index ];
}
