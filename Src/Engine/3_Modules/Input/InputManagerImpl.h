// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"
#include "InputManager.h"
#include "../../2_BuildingBlocks/Data/DataMap.h"
#include "../../2_BuildingBlocks/Math/Vector2.h"


namespace CYRED
{
	namespace NonAPI
	{
		class InputManagerImpl : public InputManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( InputManagerImpl )


		public:
			void		Initialize			( InputReceiver* receiver )	override;
			void		Finalize			()							override;
			void		ProcessEvents		()							override;

			bool		KeyDown				( int keyCode )				override;
			bool		KeyDownFirstTime	( int keyCode )				override;
			bool		KeyUp				( int keyCode )				override;
			bool		KeyUpFirstTime		( int keyCode )				override;
			Vector2		MousePosition		()							override;
			Vector2		MouseDeltaPosition	()							override;
			int			ScrollWheel			()							override;

			int			GetWindowForMouse	()							override;


		private:
			InputReceiver*		_receiver;

			DataMap<int, int>	_keyState;
			Vector2				_mousePosition;
			Vector2				_mousePrevPosition;
			int					_wheelRotation;
			int					_windowForMouse;

		};
	}
}