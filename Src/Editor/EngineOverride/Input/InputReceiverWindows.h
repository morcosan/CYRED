// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredModule_Input.h"


class QKeyEvent;
class QMouseEvent;
class QWheelEvent;


namespace CYRED
{
	class InputReceiverWindows : public InputReceiver
	{
	public:
		InputReceiverWindows() {}
		virtual ~InputReceiverWindows() {}


	public:
		void OnKeyPress		( QKeyEvent* e );
		void OnKeyRelease		( QKeyEvent* e );
		void OnMouseMove		( QMouseEvent* e, Int windowIndex );
		void OnMousePress		( QMouseEvent* e, Int windowIndex );
		void OnMouseRelease	( QMouseEvent* e, Int windowIndex );
		void OnWheelMove		( QWheelEvent* e, Int windowIndex );
		void OnFocusOut		( Int windowIndex );


	protected:
		KeyCode ToKeyCode( Int qtKey );
	};
}