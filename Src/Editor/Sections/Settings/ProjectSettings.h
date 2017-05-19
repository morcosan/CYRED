// Copyright (c) 2015-2017 Morco (www.morco.ro)
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

		static Scene* startScene;

		static String dirPathAssets;
		static String dirPathBuildWindows;
		static String dirPathBuildAndroid;
	};
}