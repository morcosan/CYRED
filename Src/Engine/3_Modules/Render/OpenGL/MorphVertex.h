// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/Math/Math.h"


#define MORPH_LIMIT 8

namespace CYRED
{
	struct MorphVertex
	{
		Vector3 position[MORPH_LIMIT];
		Vector4 color;
		Vector3 normal[MORPH_LIMIT];
		Vector2 uv;
		Vector3 tangent;
		Vector3 bitangent;
	};
}