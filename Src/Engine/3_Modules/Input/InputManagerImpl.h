// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"
#include "InputManager.h"
#include "../../2_BuildingBlocks/Data/DataMap.h"
#include "../../2_BuildingBlocks/Math/Vector2.h"


namespace CYRED
{
	namespace NotAPI
	{
		class InputManagerImpl : public InputManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( InputManagerImpl )


		public:
			void		Initialize			( InputReceiver* receiver )	override;
			void		Finalize			()							override;
			void		ProcessEvents		()							override;

			Bool		KeyDown				( KeyCode keyCode )			override;
			Bool		KeyDownFirstTime	( KeyCode keyCode )			override;
			Bool		KeyUp				( KeyCode keyCode )			override;
			Bool		KeyUpFirstTime		( KeyCode keyCode )			override;
			Vector2	CursorPosition		()							override;
			Vector2	CursorDeltaPosition	()							override;
			Int		ScrollWheel			()							override;
			Int		GetWindowForCursor	()							override;


		protected:
			InputReceiver*		_receiver;

			DataMap<Int, Int>	_keyState;
			Vector2				_cursorPosition;
			Vector2				_cursorLastPosition;
			Int					_wheelRotation;
			Int					_windowForCursor;

		};
	}
}