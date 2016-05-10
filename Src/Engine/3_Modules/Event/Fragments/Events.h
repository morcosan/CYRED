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
			ALL				= -1	// source: unknown
			, GAMEOBJECT	= 0		// source: GameObject
			, COMPONENT				// source: Component derived
			, ASSET					// source: unknown
			, SCENE					// source: unknown
			, CUSTOM				// source: unknown
			, _COUNT_
		};
	}

	namespace Enum_EventName
	{
		enum Enum
		{
			// scene events
			HIERARCHY_CHANGED
			, SCENE_SELECTED
			, GAMEOBJECT_SELECTED
			, GAMEOBJECT_RENAMED

			// component events
			, TRANSFORM_CHANGED
			, CAMERA_CHANGED
			, PARTICLE_EMITTER_CHANGED
			, MESH_RENDERING_CHANGED
			, MORPH_RENDERING_CHANGED

			// asset
			, ASSET_SELECTED
			, ASSET_CHANGED

			// custom
			, EDITOR_PROJ_SETTINGS
		};
	}

	typedef Enum_EventType::Enum	EventType;
	typedef Enum_EventName::Enum	EventName;
}