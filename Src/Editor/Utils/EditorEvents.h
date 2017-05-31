// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredModule_Event.h"


namespace CYRED
{
	namespace Enum_EditorEventType
	{
		enum Enum
		{
			EDITOR_PROJ_SETTINGS	= EventType::_COUNT_
			
			, GAMEOBJECT_SELECT

			, ASSET_SELECT

			, SCENE_SELECT

			, PREFAB_OPEN
			, PREFAB_UPDATE
			, PREFAB_CLOSE
			, PREFAB_SELECT

			, OBJECT_ISOLATE

			, _COUNT_
		};
	}

	typedef Enum_EditorEventType::Enum	EditorEventType;
}