// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredModule_Input.h"


class QKeyEvent;
class QMouseEvent;
class QWheelEvent;


namespace CYRED
{
	class InputReceiverQT : public InputReceiver
	{
	public:
		InputReceiverQT() {}
		virtual ~InputReceiverQT() {}


	public:
		void OnKeyPress		( QKeyEvent* e );
		void OnKeyRelease	( QKeyEvent* e );
		void OnMouseMove	( QMouseEvent* e, int windowIndex );
		void OnMousePress	( QMouseEvent* e, int windowIndex );
		void OnMouseRelease	( QMouseEvent* e, int windowIndex );
		void OnWheelMove	( QWheelEvent* e, int windowIndex );
		void OnFocusOut		( int windowIndex );


	private:
		KeyCode ToKeyCode( int qtKey );
	};
}