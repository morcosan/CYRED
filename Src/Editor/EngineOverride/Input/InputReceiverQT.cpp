// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "InputReceiverQT.h"
#include "CyredBuildingBlocks.h"

#include "QtGui\QKeyEvent"

using namespace CYRED;


void InputReceiverQT::OnKeyPress( QKeyEvent* e )
{
	DataUnion data;
	data.SetInt( ToKeyCode( e->key() ) );
	_eventQueue.Add( InputEvent( InputEventType::KEY_DOWN, data, INPUT_INVALID_WINDOW ) );
}


void InputReceiverQT::OnKeyRelease( QKeyEvent* e )
{
	DataUnion data;
	data.SetInt( ToKeyCode( e->key() ) );
	_eventQueue.Add( InputEvent( InputEventType::KEY_UP, data, INPUT_INVALID_WINDOW ) );
}


void InputReceiverQT::OnMouseMove( QMouseEvent* e, int windowIndex )
{
	DataUnion data;
	data.SetVector2( Vector2( e->x(), e->y() ) );
	_eventQueue.Add( InputEvent( InputEventType::MOUSE_MOVE, data, windowIndex ) );
}


void InputReceiverQT::OnMousePress( QMouseEvent* e, int windowIndex )
{
	DataUnion data;
	data.SetInt( ToKeyCode( e->button() ) );
	_eventQueue.Add( InputEvent( InputEventType::KEY_DOWN, data, windowIndex ) );
}


void InputReceiverQT::OnMouseRelease( QMouseEvent* e, int windowIndex )
{
	DataUnion data;
	data.SetInt( ToKeyCode( e->button() ) );
	_eventQueue.Add( InputEvent( InputEventType::KEY_UP, data, windowIndex ) );
}


void InputReceiverQT::OnWheelMove( QWheelEvent* e, int windowIndex )
{
	DataUnion data;
	data.SetInt( e->angleDelta().y() / 120 );
	_eventQueue.Add( InputEvent( InputEventType::WHEEL_MOVE, data, windowIndex ) );
}


void InputReceiverQT::OnFocusOut( int windowIndex )
{
	// TODO
}


KeyCode InputReceiverQT::ToKeyCode( int qtKey )
{
	switch ( qtKey )
	{
		case Qt::Key::Key_A:
			return KeyCode::A;

		case Qt::Key::Key_B:
			return KeyCode::B;

		case Qt::Key::Key_C:
			return KeyCode::C;

		case Qt::Key::Key_D:
			return KeyCode::D;

		case Qt::Key::Key_E:
			return KeyCode::E;

		case Qt::Key::Key_F:
			return KeyCode::F;

		case Qt::Key::Key_G:
			return KeyCode::G;

		case Qt::Key::Key_H:
			return KeyCode::H;

		case Qt::Key::Key_I:
			return KeyCode::I;

		case Qt::Key::Key_J:
			return KeyCode::J;

		case Qt::Key::Key_K:
			return KeyCode::K;

		case Qt::Key::Key_L:
			return KeyCode::L;

		case Qt::Key::Key_M:
			return KeyCode::M;

		case Qt::Key::Key_N:
			return KeyCode::N;

		case Qt::Key::Key_O:
			return KeyCode::O;

		case Qt::Key::Key_P:
			return KeyCode::P;

		case Qt::Key::Key_Q:
			return KeyCode::Q;

		case Qt::Key::Key_R:
			return KeyCode::R;

		case Qt::Key::Key_S:
			return KeyCode::S;

		case Qt::Key::Key_T:
			return KeyCode::T;

		case Qt::Key::Key_V:
			return KeyCode::V;

		case Qt::Key::Key_W:
			return KeyCode::W;

		case Qt::Key::Key_X:
			return KeyCode::X;

		case Qt::Key::Key_Y:
			return KeyCode::Y;

		case Qt::Key::Key_Z:
			return KeyCode::Z;

		case Qt::Key::Key_0:
			return KeyCode::NUM_0;

		case Qt::Key::Key_1:
			return KeyCode::NUM_1;

		case Qt::Key::Key_2:
			return KeyCode::NUM_2;

		case Qt::Key::Key_3:
			return KeyCode::NUM_3;

		case Qt::Key::Key_4:
			return KeyCode::NUM_4;

		case Qt::Key::Key_5:
			return KeyCode::NUM_5;

		case Qt::Key::Key_6:
			return KeyCode::NUM_6;

		case Qt::Key::Key_7:
			return KeyCode::NUM_7;

		case Qt::Key::Key_8:
			return KeyCode::NUM_8;

		case Qt::Key::Key_9:
			return KeyCode::NUM_9;

		case Qt::Key::Key_Alt:
			return KeyCode::ALT;

		case Qt::Key::Key_Control:
			return KeyCode::CTRL;

		case Qt::Key::Key_Shift:
			return KeyCode::SHIFT;

		case Qt::Key::Key_Up:
			return KeyCode::ARROW_UP;

		case Qt::Key::Key_Down:
			return KeyCode::ARROW_DOWN;

		case Qt::Key::Key_Left:
			return KeyCode::ARROW_LEFT;

		case Qt::Key::Key_Right:
			return KeyCode::ARROW_RIGHT;

		case Qt::MouseButton::LeftButton:
			return KeyCode::MOUSE_LEFT;

		case Qt::MouseButton::MiddleButton:
			return KeyCode::MOUSE_MIDDLE;

		case Qt::MouseButton::RightButton:
			return KeyCode::MOUSE_RIGHT;
	}

	//! it should not enter here
	return KeyCode();
}
