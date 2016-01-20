// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredBuildingBlocks.h"



namespace CYRED
{
	struct AppSettings
	{
		struct SceneData
		{
			String sceneName;
			String sceneDir;
			String assetdbDir;
		};

		static const Char* const FILE_PATH_CONFIG;	

		static UInt	width;		/* width of editor window						*/
		static UInt	height;		/* height of editor window						*/
		static Int	posX;		/* x position on desktop from top-left corner	*/
		static Int	posY;		/* y position on desktop						*/
		static UInt	fps;		/* desired FPS when rendering					*/
		static Bool	fullscreen;

		static String appName;

		static DataArray<SceneData> scenesData;
	};
}