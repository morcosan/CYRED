// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"
#include "Sections\KeyCode.h"


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
		virtual void	Initialize			( InputReceiver* receiver )	PURE_VIRTUAL;
		virtual void	Finalize			()							PURE_VIRTUAL;
		virtual void	ProcessEvents		()							PURE_VIRTUAL;

		virtual bool	KeyDown				( int keyCode )				PURE_VIRTUAL;
		virtual bool	KeyDownFirstTime	( int keyCode )				PURE_VIRTUAL;
		virtual bool	KeyUp				( int keyCode )				PURE_VIRTUAL;
		virtual bool	KeyUpFirstTime		( int keyCode )				PURE_VIRTUAL;
			
		virtual Vector2	MousePosition		()							PURE_VIRTUAL;
		virtual Vector2	MouseDeltaPosition	()							PURE_VIRTUAL;
		virtual int		ScrollWheel			()							PURE_VIRTUAL;

		virtual int		GetWindowForMouse	()							PURE_VIRTUAL;
	};
}