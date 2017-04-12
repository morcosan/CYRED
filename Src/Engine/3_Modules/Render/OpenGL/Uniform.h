// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"


namespace CYRED
{
	struct Uniform
	{
	public:
		static const Char*	WORLD;
		static const Char*	VIEW;
		static const Char*	PROJECTION;
		static const Char*	CAMERA_POS_WORLD;

		static const UInt	MAX_LIGHTS;
		static const Char*	LIGHTS;
		static const Char*	LIGHTS_COUNT;
		static const Char*	AMBIENT_COLOR;
	};
}