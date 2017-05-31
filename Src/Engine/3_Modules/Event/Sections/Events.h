// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"


namespace CYRED
{
	namespace Enum_EventType
	{
		enum Enum
		{
			GAMEOBJECT_RENAME = 0
			, GAMEOBJECT_CREATE
			, GAMEOBJECT_DELETE
			, GAMEOBJECT_UPDATE

			, COMPONENT_ADD
			, COMPONENT_REMOVE
			, COMPONENT_UPDATE

			, ASSET_RENAME
			, ASSET_UPDATE

			, SCENE_OPEN
			, SCENE_UPDATE
			, SCENE_CLOSE

			, CONSOLE_LOG
			, CONSOLE_ERROR

			, _COUNT_
		};
	}

	typedef Enum_EventType::Enum	EventType;
}