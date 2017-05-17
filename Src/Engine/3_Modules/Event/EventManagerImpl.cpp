// Copyright (c) 2015 Morco (www.morco.ro)
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

	for ( int i = 0; i < EventType::_COUNT_; ++i )
	{
		_listeners.Add( DataArray<IEventListener*>() );
	}
}


void EventManagerImpl::Finalize()
{
	ASSERT( _isInitialized );
}


void EventManagerImpl::RegisterListener( EventType eType, IEventListener* listener )
{
	ASSERT( _isInitialized );

	if ( eType == EventType::ALL )
	{
		for ( int i = 0; i < EventType::_COUNT_; ++i )
		{
			_AddListener( i, listener );
		}
	}
	else
	{
		_AddListener( eType, listener );
	}
}


void EventManagerImpl::UnregisterListener( EventType eType, IEventListener* listener )
{
	ASSERT( _isInitialized );

	if ( eType == EventType::ALL )
	{
		for ( int i = 0; i < EventType::_COUNT_; ++i )
		{
			_RemoveListener( i, listener );
		}
	}
	else
	{
		_RemoveListener( eType, listener );
	}
}


void EventManagerImpl::EmitEvent( EventType eType, void* eData )
{
	ASSERT( _isInitialized );

	for ( int i = 0; i < EventType::_COUNT_; ++i )
	{
		if ( eType == i )
		{
			for ( int j = 0; j < _listeners[i].Size(); ++j )
			{
				_listeners[i][j]->OnEvent( eType, eData );
			}
			break;
		}
	}
}


void EventManagerImpl::_AddListener( int eType, IEventListener* listener )
{
	for ( int i = 0; i < _listeners[eType].Size(); ++i )
	{
		if ( _listeners[eType][i] == listener )
		{
			return;
		}
	}
	_listeners[eType].Add( listener );
}


void EventManagerImpl::_RemoveListener( int eType, IEventListener* listener )
{
	for ( int i = 0; i < _listeners[eType].Size(); ++i )
	{
		if ( _listeners[eType][i] == listener )
		{
			_listeners[eType].Erase( i );
			return;
		}
	}
}
