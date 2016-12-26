// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "InputManagerImpl.h"
#include "Fragments\InputReceiver.h"


using namespace CYRED;
using namespace NotAPI;


//! deferred definition of RenderManager
DEFINE_LOCAL_SINGLETON( InputManager, InputManagerImpl )

DEFINE_LOCAL_SINGLETON_IMPL( InputManagerImpl )



void InputManagerImpl::Initialize( InputReceiver* receiver )
{
	ASSERT( !_isInitialized );
	_isInitialized = true;

	_receiver = receiver;

	_windowForCursor = INPUT_INVALID_WINDOW;
	_wheelRotation = 0;

	for ( UInt i = 0; i < KeyCode::_COUNT_; ++i )
	{
		_keyState.Set( i, INPUT_CONTINUOUS_UP );
	}
}


void InputManagerImpl::Finalize()
{
	if ( !_isInitialized )
	{
		return;
	}
}


void InputManagerImpl::ProcessEvents()
{
	ASSERT( _isInitialized );

	//! update from last frame
	{
		Iterator<Int, Int> iter = _keyState.GetIterator();

		while ( iter.HasNext() )
		{
			if ( iter.GetValue() == INPUT_FIRST_TIME_DOWN ) 
			{
				_keyState.Set( iter.GetKey(), INPUT_CONTINUOUS_DOWN );
			} 
			if ( iter.GetValue() == INPUT_FIRST_TIME_UP ) 
			{
				_keyState.Set( iter.GetKey(), INPUT_CONTINUOUS_UP );
			} 

			iter.Next();
		}

		_cursorLastPosition = _cursorPosition;
		_wheelRotation = 0;
	}

	//! process the new events
	{
		UInt totalEvents = _receiver->GetEventsCount();

		for ( UInt i = 0; i < totalEvents; ++i )
		{
			InputEvent event = _receiver->GetEventAt( i );

			switch ( event.type )
			{
				case InputEventType::KEY_DOWN:
					_keyState.Set( event.data.GetInt(), INPUT_FIRST_TIME_DOWN );
					break;

				case InputEventType::KEY_UP:
					_keyState.Set( event.data.GetInt(), INPUT_FIRST_TIME_UP );
					break;

				case InputEventType::MOUSE_MOVE:
					_cursorPosition = event.data.GetVector2();
					break;

				case InputEventType::WHEEL_MOVE:
					_wheelRotation = event.data.GetInt();
					break;
			}

			if ( event.window != INPUT_INVALID_WINDOW )
			{
				_windowForCursor = event.window;
			}
		}

		//! remove all processed events
		_receiver->ClearQueue();
	}
}


Bool InputManagerImpl::KeyDown( KeyCode keyCode )
{
	ASSERT( _isInitialized );
	
	Int state = _keyState.Get( keyCode );
	return ( state == INPUT_CONTINUOUS_DOWN || state == INPUT_FIRST_TIME_DOWN );
}


Bool InputManagerImpl::KeyDownFirstTime( KeyCode keyCode )
{
	ASSERT( _isInitialized );
	
	Int state = _keyState.Get( keyCode );
	return ( state == INPUT_FIRST_TIME_DOWN );
}


Bool InputManagerImpl::KeyUp( KeyCode keyCode )
{
	ASSERT( _isInitialized );
	
	Int state = _keyState.Get( keyCode );
	return ( state == INPUT_CONTINUOUS_UP || state == INPUT_FIRST_TIME_UP );
}


Bool InputManagerImpl::KeyUpFirstTime( KeyCode keyCode )
{
	ASSERT( _isInitialized );
	
	Int state = _keyState.Get( keyCode );
	return ( state == INPUT_FIRST_TIME_UP );
}


Vector2 InputManagerImpl::CursorPosition()
{
	ASSERT( _isInitialized );
	
	return _cursorPosition;
}


Vector2 InputManagerImpl::CursorDeltaPosition()
{
	ASSERT( _isInitialized );
	
	return (_cursorPosition - _cursorLastPosition);
}


Int InputManagerImpl::ScrollWheel()
{
	ASSERT( _isInitialized );
	
	return _wheelRotation;
}


Int InputManagerImpl::GetWindowForCursor()
{
	ASSERT( _isInitialized );
	
	return _windowForCursor;
}

