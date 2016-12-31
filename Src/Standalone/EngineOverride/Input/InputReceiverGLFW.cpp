// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "InputReceiverGLFW.h"

#include "CyredBuildingBlocks.h"

#include "GLFW\Include\glfw3.h"


using namespace CYRED;


// define singleton
DEFINE_LOCAL_SINGLETON_IMPL( InputReceiverGLFW )


void InputReceiverGLFW::GLFW_KeyCallback( GLFWwindow* window, Int key, Int scancode, 
										  Int action, Int mods )
{
	// check close window event
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ) {
		glfwSetWindowShouldClose( window, GL_TRUE );
		return;
	}

	// on key pressed
	if ( action == GLFW_PRESS ) {
		DataUnion data;
		data.SetInt( InputReceiverGLFW::ToKeyCode( key ) );
		InputReceiverGLFW::Singleton()->AddEventToQueue( 
			InputEvent( InputEventType::KEY_DOWN, data, INPUT_INVALID_WINDOW ) 
		);
	}
	else if (action == GLFW_RELEASE) {
		DataUnion data;
		data.SetInt( InputReceiverGLFW::ToKeyCode( key ) );
		InputReceiverGLFW::Singleton()->AddEventToQueue( 
			InputEvent( InputEventType::KEY_UP, data, INPUT_INVALID_WINDOW ) 
		);
	}

	// check modifiers
	InputReceiverGLFW::Singleton()->CheckModifiers( mods );
}


void InputReceiverGLFW::GLFW_ButtonCallback( GLFWwindow* window, Int button, Int action, Int mods )
{
	// on key pressed
	if ( action == GLFW_PRESS ) {
		DataUnion data;
		data.SetInt( InputReceiverGLFW::ToKeyCode( button ) );
		InputReceiverGLFW::Singleton()->AddEventToQueue( 
			InputEvent( InputEventType::KEY_DOWN, data, INPUT_INVALID_WINDOW ) 
		);
	}
	else if (action == GLFW_RELEASE) {
		DataUnion data;
		data.SetInt( InputReceiverGLFW::ToKeyCode( button ) );
		InputReceiverGLFW::Singleton()->AddEventToQueue( 
			InputEvent( InputEventType::KEY_UP, data, INPUT_INVALID_WINDOW ) 
		);
	}

	// check modifiers
	InputReceiverGLFW::Singleton()->CheckModifiers( mods );
}


void InputReceiverGLFW::GLFW_CursorCallback( GLFWwindow* window, Double xpos, Double ypos )
{
	DataUnion data;
	data.SetVector2( Vector2( CAST_S(Float, xpos), CAST_S(Float, ypos) ) );
	InputReceiverGLFW::Singleton()->AddEventToQueue( 
		InputEvent( InputEventType::MOUSE_MOVE, data, INPUT_INVALID_WINDOW ) 
	);
}


void InputReceiverGLFW::GLFW_ScrollCallback( GLFWwindow* window, Double xoffset, Double yoffset )
{
	DataUnion data;
	data.SetInt( CAST_S(Int, yoffset) );
	InputReceiverGLFW::Singleton()->AddEventToQueue( 
		InputEvent( InputEventType::WHEEL_MOVE, data, INPUT_INVALID_WINDOW ) 
	);
}


void InputReceiverGLFW::GLFW_FocusCallback( GLFWwindow* window, Int entered )
{
	if ( entered == GL_TRUE )	{
		// enter window
	}
	else {
		// leave window
	}
}


void InputReceiverGLFW::AddEventToQueue( InputEvent event )
{
	_eventQueue.Add( event );
}


void InputReceiverGLFW::CheckModifiers( Int mods )
{
	// modifiers cannot check first time up/down
	// check ALT
	if ( (mods & GLFW_MOD_ALT) != 0 ) {
		DataUnion data;
		data.SetInt( KeyCode::ALT );
		InputReceiverGLFW::Singleton()->AddEventToQueue( 
			InputEvent( InputEventType::KEY_DOWN, data, INPUT_INVALID_WINDOW ) 
		);
	}
	else {
		DataUnion data;
		data.SetInt( KeyCode::ALT );
		InputReceiverGLFW::Singleton()->AddEventToQueue( 
			InputEvent( InputEventType::KEY_UP, data, INPUT_INVALID_WINDOW ) 
		);
	}

	// check CTRL
	if ( (mods & GLFW_MOD_CONTROL) != 0 ) {
		DataUnion data;
		data.SetInt( KeyCode::CTRL );
		InputReceiverGLFW::Singleton()->AddEventToQueue( 
			InputEvent( InputEventType::KEY_DOWN, data, INPUT_INVALID_WINDOW ) 
		);
	}
	else {
		DataUnion data;
		data.SetInt( KeyCode::CTRL );
		InputReceiverGLFW::Singleton()->AddEventToQueue( 
			InputEvent( InputEventType::KEY_UP, data, INPUT_INVALID_WINDOW ) 
		);
	}

	// check SHIFT
	if ( (mods & GLFW_MOD_SHIFT) != 0 ) {
		DataUnion data;
		data.SetInt( KeyCode::SHIFT );
		InputReceiverGLFW::Singleton()->AddEventToQueue( 
			InputEvent( InputEventType::KEY_DOWN, data, INPUT_INVALID_WINDOW ) 
		);
	}
	else {
		DataUnion data;
		data.SetInt( KeyCode::SHIFT );
		InputReceiverGLFW::Singleton()->AddEventToQueue( 
			InputEvent( InputEventType::KEY_UP, data, INPUT_INVALID_WINDOW ) 
		);
	}
}



KeyCode InputReceiverGLFW::ToKeyCode( Int glfwKey )
{
	switch ( glfwKey ) {
		case GLFW_KEY_A:
			return KeyCode::A;

		case GLFW_KEY_B:
			return KeyCode::B;

		case GLFW_KEY_C:
			return KeyCode::C;

		case GLFW_KEY_D:
			return KeyCode::D;

		case GLFW_KEY_E:
			return KeyCode::E;

		case GLFW_KEY_F:
			return KeyCode::F;

		case GLFW_KEY_G:
			return KeyCode::G;

		case GLFW_KEY_H:
			return KeyCode::H;

		case GLFW_KEY_I:
			return KeyCode::I;

		case GLFW_KEY_J:
			return KeyCode::J;

		case GLFW_KEY_K:
			return KeyCode::K;

		case GLFW_KEY_L:
			return KeyCode::L;

		case GLFW_KEY_M:
			return KeyCode::M;

		case GLFW_KEY_N:
			return KeyCode::N;

		case GLFW_KEY_O:
			return KeyCode::O;

		case GLFW_KEY_P:
			return KeyCode::P;

		case GLFW_KEY_Q:
			return KeyCode::Q;

		case GLFW_KEY_R:
			return KeyCode::R;

		case GLFW_KEY_S:
			return KeyCode::S;

		case GLFW_KEY_T:
			return KeyCode::T;

		case GLFW_KEY_V:
			return KeyCode::V;

		case GLFW_KEY_W:
			return KeyCode::W;

		case GLFW_KEY_X:
			return KeyCode::X;

		case GLFW_KEY_Y:
			return KeyCode::Y;

		case GLFW_KEY_Z:
			return KeyCode::Z;

		case GLFW_KEY_0:
			return KeyCode::NUM_0;

		case GLFW_KEY_1:
			return KeyCode::NUM_1;

		case GLFW_KEY_2:
			return KeyCode::NUM_2;

		case GLFW_KEY_3:
			return KeyCode::NUM_3;

		case GLFW_KEY_4:
			return KeyCode::NUM_4;

		case GLFW_KEY_5:
			return KeyCode::NUM_5;

		case GLFW_KEY_6:
			return KeyCode::NUM_6;

		case GLFW_KEY_7:
			return KeyCode::NUM_7;

		case GLFW_KEY_8:
			return KeyCode::NUM_8;

		case GLFW_KEY_9:
			return KeyCode::NUM_9;

		case GLFW_KEY_LEFT_ALT:
		case GLFW_KEY_RIGHT_ALT:
			return KeyCode::ALT;

		case GLFW_KEY_LEFT_CONTROL:
		case GLFW_KEY_RIGHT_CONTROL:
			return KeyCode::CTRL;

		case GLFW_KEY_LEFT_SHIFT:
		case GLFW_KEY_RIGHT_SHIFT:
			return KeyCode::SHIFT;

		case GLFW_KEY_UP:
			return KeyCode::ARROW_UP;

		case GLFW_KEY_DOWN:
			return KeyCode::ARROW_DOWN;

		case GLFW_KEY_LEFT:
			return KeyCode::ARROW_LEFT;

		case GLFW_KEY_RIGHT:
			return KeyCode::ARROW_RIGHT;

		case GLFW_MOUSE_BUTTON_1:
			return KeyCode::MOUSE_LEFT;

		case GLFW_MOUSE_BUTTON_2:
			return KeyCode::MOUSE_MIDDLE;

		case GLFW_MOUSE_BUTTON_3:
			return KeyCode::MOUSE_RIGHT;
	}

	ASSERT( FALSE );	//! it should not enter here
	return KeyCode();
}