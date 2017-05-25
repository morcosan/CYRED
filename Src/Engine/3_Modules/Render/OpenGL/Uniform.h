// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"


namespace CYRED
{
	struct Uniform
	{
	public:
		static cchar*		WORLD;
		static cchar*		VIEW;
		static cchar*		PROJECTION;
		static cchar*		CAMERA_POS_WORLD;
		static cchar*		UID;

		static const int	MAX_LIGHTS;
		static cchar*		LIGHTS;
		static cchar*		LIGHTS_COUNT;
		static cchar*		AMBIENT_COLOR;
	};
}