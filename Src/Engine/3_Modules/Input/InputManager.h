// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"
#include "Fragments\KeyCode.h"


namespace CYRED
{
	class InputReceiver;
	struct Vector2;
}


namespace CYRED
{
	//!		controls the input flow
	ABSTRACT class DLL InputManager
	{
		DECLARE_LOCAL_SINGLETON( InputManager )


	public:
		virtual void Initialize	( InputReceiver* receiver )	PURE_VIRTUAL;
		virtual void Finalize		()							PURE_VIRTUAL;
		virtual void ProcessEvents()							PURE_VIRTUAL;

		virtual Bool KeyDown			( KeyCode keyCode )	PURE_VIRTUAL;
		virtual Bool KeyDownFirstTime	( KeyCode keyCode )	PURE_VIRTUAL;
		virtual Bool KeyUp			( KeyCode keyCode )	PURE_VIRTUAL;
		virtual Bool KeyUpFirstTime	( KeyCode keyCode )	PURE_VIRTUAL;

		virtual Vector2	CursorPosition		()	PURE_VIRTUAL;
		virtual Vector2	CursorDeltaPosition	()	PURE_VIRTUAL;
		virtual Int		ScrollWheel			()	PURE_VIRTUAL;

		//! returns the index for the window where cursor is inside
		virtual Int		GetWindowForCursor	()	PURE_VIRTUAL;

	};
}