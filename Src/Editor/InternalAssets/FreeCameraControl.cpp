// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "FreeCameraControl.h"
#include "CyredModule_Input.h"
#include "CyredModule_Time.h"


using namespace CYRED;



FreeCameraControl::FreeCameraControl( GameObject * gameObject )
	: Component( gameObject )
{
}


void FreeCameraControl::_OnStart( bool isRuntime )
{
	_transform = _gameObject->GetComponent<Transform>();
}


void FreeCameraControl::_OnUpdate( bool isRuntime )
{
	// works only in editor mode
	if ( !isRuntime ) {
		InputManager* inputManager = InputManager::Singleton();

		int windowForCursor = inputManager->GetWindowForCursor();
		float deltaTime = TimeManager::Singleton()->GetDeltaTime();

		if ( myWindows.Has( windowForCursor ) && myWindows.Get( windowForCursor ) )
		{
			if ( inputManager->KeyDown( KeyCode::MOUSE_RIGHT ) )
			{
				Vector2 cursorDelta = inputManager->CursorDeltaPosition();

				_transform->RotateByWorld( Vector3( 0, - cursorDelta.x * rotateSpeed * deltaTime, 0 ) );
				_transform->RotateByLocal( Vector3( - cursorDelta.y * rotateSpeed * deltaTime, 0, 0 ) );
			}

			if ( inputManager->KeyDown( KeyCode::MOUSE_MIDDLE ) )
			{
				Vector2 cursorDelta = inputManager->CursorDeltaPosition();

				_transform->TranslateByLocal( Vector3( - cursorDelta.x * panSpeed * deltaTime,
														cursorDelta.y * panSpeed * deltaTime,
														0 ) );
			}

			int scrollWheel = inputManager->ScrollWheel();
			if ( scrollWheel != 0 )
			{
				float zoom = zoomSpeed * (- scrollWheel) * deltaTime;
				_transform->TranslateByLocal( Vector3( 0, 0, zoom ) );
			}
		}
	}
}
