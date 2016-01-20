// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "EventManagerImpl.h"
#include "Fragments\EventListener.h"


using namespace CYRED;
using namespace NotAPI;


//! deferred definition of EventManager
DEFINE_LOCAL_SINGLETON( EventManager, EventManagerImpl )

DEFINE_LOCAL_SINGLETON_IMPL( EventManagerImpl )



void EventManagerImpl::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = true;

	for ( UInt i = 0; i < EventType::_COUNT_; ++i )
	{
		_listeners.Add( DataArray<EventListener*>() );
	}
}


void EventManagerImpl::Finalize()
{
	if ( !_isInitialized )
	{
		return;
	}
}


void EventManagerImpl::RegisterListener( EventType eType, EventListener* listener )
{
	ASSERT( _isInitialized );

	if ( eType == EventType::ALL )
	{
		for ( UInt i = 0; i < EventType::_COUNT_; ++i )
		{
			_AddListener( i, listener );
		}
	}
	else
	{
		_AddListener( eType, listener );
	}
}


void EventManagerImpl::UnregisterListener( EventType eType, EventListener* listener )
{
	ASSERT( _isInitialized );

	if ( eType == EventType::ALL )
	{
		for ( UInt i = 0; i < EventType::_COUNT_; ++i )
		{
			_RemoveListener( i, listener );
		}
	}
	else
	{
		_RemoveListener( eType, listener );
	}
}


void EventManagerImpl::EmitEvent( EventType eType, EventName eName, void* eSource )
{
	ASSERT( _isInitialized );

	for ( UInt i = 0; i < EventType::_COUNT_; ++i )
	{
		if ( eType == i )
		{
			for ( UInt j = 0; j < _listeners[i].Size(); ++j )
			{
				_listeners[i][j]->OnEvent( eType, eName, eSource );
			}
			break;
		}
	}
}


void EventManagerImpl::_AddListener( UInt eType, EventListener* listener )
{
	for ( UInt i = 0; i < _listeners[eType].Size(); ++i )
	{
		if ( _listeners[eType][i] == listener )
		{
			return;
		}
	}
	_listeners[eType].Add( listener );
}


void EventManagerImpl::_RemoveListener( UInt eType, EventListener* listener )
{
	for ( UInt i = 0; i < _listeners[eType].Size(); ++i )
	{
		if ( _listeners[eType][i] == listener )
		{
			_listeners[eType].Erase( i );
			return;
		}
	}
}
