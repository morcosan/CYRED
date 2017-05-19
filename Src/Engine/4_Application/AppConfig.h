// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
		static const char* const FILE_PATH_CONFIG;	
		static const char* const DIR_PATH_DATA;	


	public:
		int		width;		/* width of editor window						*/
		int		height;		/* height of editor window						*/
		int			posX;		/* x position on desktop from top-left corner	*/
		int			posY;		/* y position on desktop						*/
		int		fps;		/* desired FPS when rendering					*/
		bool		fullscreen;

		String		appName;

		AssetConfig	startScene;

		DataArray<AssetConfig>	assetMeshes;
		DataArray<AssetConfig>	assetTextures;
		DataArray<AssetConfig>	assetMorphs;
		DataArray<AssetConfig>	assetScenes;
		DataArray<AssetConfig>	assetShaders;
		DataArray<AssetConfig>	assetMaterials;
		DataArray<AssetConfig>	assetScripts;
	};
}