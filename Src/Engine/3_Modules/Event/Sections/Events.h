// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"


namespace CYRED
{
	namespace Enum_EventType
	{
		enum Enum
		{
			ALL						= -1
			, CHANGE_HIERARCHY		= 0
			
			, SELECT_SCENE
			, SELECT_GAMEOBJECT
			, SELECT_ASSET

			, RENAME_GAMEOBJECT
			
			, CHANGE_GAMEOBJECT
			, CHANGE_TRANSFORM
			, CHANGE_CAMERA
			, CHANGE_PARTICLE_EMITTER
			, CHANGE_MESH_RENDERING
			, CHANGE_MORPH_RENDERING
			, CHANGE_SCRIPTER
			, CHANGE_ASSET

			, CONSOLE_LOG
			, CONSOLE_ERROR

			, EDITOR_PROJ_SETTINGS

			, _COUNT_
		};
	}

	typedef Enum_EventType::Enum	EventType;
}