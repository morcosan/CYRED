// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"


namespace CYRED
{
	struct Uniform
	{
	public:
		static const char*	WORLD;
		static const char*	VIEW;
		static const char*	PROJECTION;
		static const char*	CAMERA_POS_WORLD;

		static const int	MAX_LIGHTS;
		static const char*	LIGHTS;
		static const char*	LIGHTS_COUNT;
		static const char*	AMBIENT_COLOR;
	};
}