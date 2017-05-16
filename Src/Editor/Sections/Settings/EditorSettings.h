// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredBuildingBlocks.h"

#include "../Panel.h"


namespace CYRED
{
	namespace Enum_PanelSplitType
	{
		enum Enum
		{
			HORIZONTAL
			, VERTICAL
		};
	}
	typedef Enum_PanelSplitType::Enum	PanelSplitType;
}


namespace CYRED
{
	struct EditorSettings
	{
		static const Char* const EDITOR_NAME;			/* main title of editor			*/

		static const Char* const FILE_PATH_CONFIG;		

		static const Char* const DIR_PATH_SKINS;		/* directory for skins			*/
		static const Char* const DIR_PATH_ICONS_ASSETS;		
		static const Char* const DIR_PATH_ICONS_LAYOUT;		

		static const Char* const DIR_DEFALUT_ASSETS;		
		static const Char* const DIR_PATH_DEFALUT_ASSETS;		

		static const Char* const MSG_EDITOR_STARTED;	/* message when editor starts	*/

		static const Char* const SKIN_DEFAULT_BLACK;	/* name of the file for skin	*/
		static const Char* const SKIN_DEFAULT_WHITE;	/* name of the file for skin	*/

		static const Char* const FILE_FORMAT_SKINS;		/* the format of the skin file	*/

		struct PanelData
		{
			PanelType		type;
			PanelType		splitFrom;
			PanelSplitType	splitType;
			UInt			viewportIndex;
		};

		static Bool fullscreen;
		static UInt	width;		/* width of editor window						*/
		static UInt	height;		/* height of editor window						*/
		static Int	posX;		/* x position on desktop from top-left corner	*/
		static Int	posY;		/* y position on desktop						*/
		static UInt	fps;		/* desired FPS when rendering					*/

		static String projectName;
		static String projectPath;	/* path to project .cyredproj file	*/	

		static DataArray<PanelData>	panels;
	};
}