// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "InputManagerImpl.h"
#include "Sections\InputReceiver.h"


using namespace CYRED;
using namespace NonAPI;


//! deferred definition of RenderManager
DEFINE_LOCAL_SINGLETON( InputManager, InputManagerImpl )

DEFINE_LOCAL_SINGLETON_IMPL( InputManagerImpl )



void InputManagerImpl::Initialize( InputReceiver* receiver )
{
	ASSERT( !_isInitialized );
	_isInitialized = true;

	_receiver = receiver;

	_windowForMouse = INPUT_INVALID_WINDOW;
	_wheelRotation = 0;

	for ( int i = 0; i < KeyCode::_COUNT_; ++i )
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
		Iterator<int, int> iter = _keyState.GetIterator();

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

		_mousePrevPosition = _mousePosition;
		_wheelRotation = 0;
	}

	//! process the new events
	{
		int totalEvents = _receiver->GetEventsCount();

		for ( int i = 0; i < totalEvents; ++i )
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
					_mousePosition = event.data.GetVector2();
					break;

				case InputEventType::WHEEL_MOVE:
					_wheelRotation = event.data.GetInt();
					break;
			}

			if ( event.window != INPUT_INVALID_WINDOW )
			{
				_windowForMouse = event.window;
			}
		}

		//! remove all processed events
		_receiver->ClearQueue();
	}
}


bool InputManagerImpl::KeyDown( int keyCode )
{
	ASSERT( _isInitialized );
	
	int state = _keyState.Get( keyCode );
	return ( state == INPUT_CONTINUOUS_DOWN || state == INPUT_FIRST_TIME_DOWN );
}


bool InputManagerImpl::KeyDownFirstTime( int keyCode )
{
	ASSERT( _isInitialized );
	
	int state = _keyState.Get( keyCode );
	return ( state == INPUT_FIRST_TIME_DOWN );
}


bool InputManagerImpl::KeyUp( int keyCode )
{
	ASSERT( _isInitialized );
	
	int state = _keyState.Get( keyCode );
	return ( state == INPUT_CONTINUOUS_UP || state == INPUT_FIRST_TIME_UP );
}


bool InputManagerImpl::KeyUpFirstTime( int keyCode )
{
	ASSERT( _isInitialized );
	
	int state = _keyState.Get( keyCode );
	return ( state == INPUT_FIRST_TIME_UP );
}


Vector2 InputManagerImpl::MousePosition()
{
	ASSERT( _isInitialized );
	
	return _mousePosition;
}


Vector2 InputManagerImpl::MouseDeltaPosition()
{
	ASSERT( _isInitialized );
	
	return (_mousePosition - _mousePrevPosition);
}


int InputManagerImpl::ScrollWheel()
{
	ASSERT( _isInitialized );
	
	return _wheelRotation;
}


int InputManagerImpl::GetWindowForMouse()
{
	ASSERT( _isInitialized );
	
	return _windowForMouse;
}

