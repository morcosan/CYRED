// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/Data/DataUnion.h"


namespace CYRED
{
	namespace Enum_InputEventType
	{
		enum Enum
		{
			KEY_DOWN
			, KEY_UP
			, MOUSE_MOVE
			, WHEEL_MOVE
		};
	}

	typedef Enum_InputEventType::Enum InputEventType;
}


namespace CYRED
{
	struct DLL InputEvent
	{
		InputEventType	type;
		DataUnion		data;
		Int				window;


		InputEvent( InputEventType newType, DataUnion& newData, Int windowIndex )
			: type( newType )
			, data( newData )
			, window ( windowIndex )
		{
		}
	};
}