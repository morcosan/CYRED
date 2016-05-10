// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../1_Required/Required.h"
#include "../2_BuildingBlocks/String/String.h"
#include "../2_BuildingBlocks/Data/DataArray.h"


namespace CYRED
{
	struct AssetDB
	{
		String name;
		String dir;
	};
}


namespace CYRED
{
	class DLL AppConfig
	{
	public:
		static const Char* const FILE_PATH_CONFIG;	


	public:
		UInt	width;		/* width of editor window						*/
		UInt	height;		/* height of editor window						*/
		Int		posX;		/* x position on desktop from top-left corner	*/
		Int		posY;		/* y position on desktop						*/
		UInt	fps;		/* desired FPS when rendering					*/
		Bool	fullscreen;

		String	appName;

		DataArray<AssetDB> assetDB;
	};
}