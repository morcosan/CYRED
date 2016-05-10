// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredApplication.h"
#include "CyredBuildingBlocks.h"

namespace CYRED
{
	class Scene;
}


namespace CYRED
{
	struct ProjectSettings
	{
		static AppConfig appConfig;

		static DataArray<Scene*> scenes;

		static String dirPathAssets;
		static String dirPathBuild;
	};
}