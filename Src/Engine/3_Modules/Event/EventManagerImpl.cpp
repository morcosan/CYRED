// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "EventManagerImpl.h"
#include "Sections\IEventListener.h"


using namespace CYRED;
using namespace NotAPI;


//! deferred definition of EventManager
DEFINE_LOCAL_SINGLETON( EventManager, EventManagerImpl )
DEFINE_LOCAL_SINGLETON_IMPL( EventManagerImpl )


void EventManagerImpl::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = true;

	// clear list, just in case
	_listeners.Clear();
}


void EventManagerImpl::Finalize()
{
	ASSERT( _isInitialized );
	_isInitialized = FALSE;
}


void EventManagerImpl::RegisterListener( IEventListener* listener, int eventType )
{
	ASSERT( _isInitialized );

	// if event does not exist, add it
	if ( !_listeners.Has( eventType ) ) {
		_listeners.Set( eventType, DataArray<IEventListener*>() );
	}
	// add listener to list
	_listeners.Get( eventType ).Add( listener );
}


void EventManagerImpl::UnregisterListener(  IEventListener* listener, int eventType )
{
	ASSERT( _isInitialized );

	// check if event exists
	if ( !_listeners.Has( eventType ) ) {
		return;
	}
	
	// search for listener
	DataArray<IEventListener*>& list = _listeners.Get( eventType );
	for ( int i = 0; i < list.Size(); i++ ) {
		if ( list[i] == listener ) {
			list.Erase( i );
			return;
		}
	}
}


void EventManagerImpl::EmitEvent( int eventType, void* eventData )
{
	ASSERT( _isInitialized );

	// check if event exists
	if ( !_listeners.Has( eventType ) ) {
		return;
	}

	// send event to all listeners
	DataArray<IEventListener*>& list = _listeners.Get( eventType );
	for ( int i = 0; i < list.Size(); i++ ) {
		list[i]->OnEvent( eventType, eventData );
	}
}
