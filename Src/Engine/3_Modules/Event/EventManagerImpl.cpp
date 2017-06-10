// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "EventManagerImpl.h"
#include "Sections\IEventListener.h"


using namespace CYRED;
using namespace NonAPI;


//! deferred definition of EventManager
DEFINE_LOCAL_SINGLETON( EventManager, EventManagerImpl )
DEFINE_LOCAL_SINGLETON_IMPL( EventManagerImpl )


void EventManagerImpl::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = true;
}


void EventManagerImpl::Update()
{
	ASSERT( _isInitialized );

	int eventsCount = _pushedEvents.Size();
	if ( eventsCount > 0 ) {
		// send all async events
		for ( int i = 0; i < eventsCount; i++ ) {
			_SendEvent( &_listenersAsyncAll, _pushedEvents[i].eventType, _pushedEvents[i].eventData );
		}

		// send last async events
		for ( int i = eventsCount - 1; i >= 0; i-- ) {
			// check if the event was sent before
			bool wasSent = false;
			for ( int j = eventsCount - 1; j > i; j-- ) {
				if ( _pushedEvents[j].eventType == _pushedEvents[i].eventType ) {
					wasSent = true;
					break;
				}
			}

			// if new, send it
			if ( !wasSent ) {
				_SendEvent( &_listenersAsyncLast, _pushedEvents[i].eventType, _pushedEvents[i].eventData );
			}
		}

		// clear list
		_pushedEvents.Clear();
	}
}


void EventManagerImpl::Finalize()
{
	ASSERT( _isInitialized );
	_isInitialized = FALSE;
}


void EventManagerImpl::Register( IEventListener* listener, int eventType, EventListenMode mode )
{
	ASSERT( _isInitialized );

	DataMap<int, DataArray<IEventListener*>>* listeners = NULL;
	switch ( mode ) {
		case EventListenMode::SYNC:			listeners = &_listenersSync;		break;
		case EventListenMode::ASYNC_ALL:	listeners = &_listenersAsyncAll;	break;
		case EventListenMode::ASYNC_LAST:	listeners = &_listenersAsyncLast;	break;
	}

	// if event does not exist, add it
	if ( !listeners->Has( eventType ) ) {
		listeners->Set( eventType, DataArray<IEventListener*>() );
	}
	// add listener to list
	listeners->Get( eventType ).Add( listener );
}


void EventManagerImpl::Unregister( IEventListener* listener, int eventType, EventListenMode mode )
{
	ASSERT( _isInitialized );


	DataMap<int, DataArray<IEventListener*>>* listeners = NULL;
	switch ( mode ) {
		case EventListenMode::SYNC:			listeners = &_listenersSync;		break;
		case EventListenMode::ASYNC_ALL:	listeners = &_listenersAsyncAll;	break;
		case EventListenMode::ASYNC_LAST:	listeners = &_listenersAsyncLast;	break;
	}

	// check if event exists
	if ( !listeners->Has( eventType ) ) {
		return;
	}
	
	// search for listener
	DataArray<IEventListener*>& list = listeners->Get( eventType );
	for ( int i = 0; i < list.Size(); i++ ) {
		if ( list[i] == listener ) {
			list.Erase( i );
			return;
		}
	}
}


// this is done synchronously
void EventManagerImpl::EmitEvent( int eventType, void* eventData )
{
	ASSERT( _isInitialized );

	// send event right away
	_SendEvent( &_listenersSync, eventType, eventData );
}


// this is done asynchronously
void EventManagerImpl::PushEvent( int eventType, void* eventData )
{
	// add event to list
	_pushedEvents.Add( _PushedEvent {
		eventType,
		eventData
	} );
}


void EventManagerImpl::_SendEvent( DataMap<int, DataArray<IEventListener*>>* listeners, 
								   int eventType, void* eventData )
{
	// send event to registered listeners
	if ( listeners->Has( eventType ) ) {
		DataArray<IEventListener*>& list = listeners->Get( eventType );
		for ( int i = 0; i < list.Size(); i++ ) {
			list[i]->OnEvent( eventType, eventData );
		}
	}

	// send this event to all
	if ( eventType != EventType::ALL && listeners->Has( EventType::ALL ) ) {
		DataArray<IEventListener*>& list = listeners->Get( EventType::ALL );
		for ( int i = 0; i < list.Size(); i++ ) {
			list[i]->OnEvent( eventType, eventData );
		}
	}
}

