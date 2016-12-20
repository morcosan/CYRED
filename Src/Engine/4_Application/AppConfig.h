// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../1_Required/Required.h"
#include "../2_BuildingBlocks/String/String.h"
#include "../2_BuildingBlocks/Data/DataArray.h"



namespace CYRED
{
	class DLL AppConfig
	{
	public:
		class AssetConfig
		{
		public:
			String name;
			String path;
		};


	public:
		static const Char* const FILE_PATH_CONFIG;	
		static const Char* const DIR_PATH_DATA;	


	public:
		UInt		width;		/* width of editor window						*/
		UInt		height;		/* height of editor window						*/
		Int			posX;		/* x position on desktop from top-left corner	*/
		Int			posY;		/* y position on desktop						*/
		UInt		fps;		/* desired FPS when rendering					*/
		Bool		fullscreen;

		String		appName;

		AssetConfig	startScene;

		DataArray<AssetConfig>	assetMeshes;
		DataArray<AssetConfig>	assetTextures;
		DataArray<AssetConfig>	assetMorphs;
		DataArray<AssetConfig>	assetScenes;
		DataArray<AssetConfig>	assetShaders;
		DataArray<AssetConfig>	assetMaterials;
	};
}