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
		static const char* const EDITOR_NAME;			/* main title of editor			*/

		static const char* const FILE_PATH_CONFIG;		

		static const char* const DIR_PATH_SKINS;		/* directory for skins			*/
		static const char* const DIR_PATH_ICONS_ASSETS;		
		static const char* const DIR_PATH_ICONS_LAYOUT;		

		static const char* const DIR_DEFALUT_ASSETS;		
		static const char* const DIR_PATH_DEFALUT_ASSETS;		

		static const char* const MSG_EDITOR_STARTED;	/* message when editor starts	*/

		static const char* const SKIN_DEFAULT_BLACK;	/* name of the file for skin	*/
		static const char* const SKIN_DEFAULT_WHITE;	/* name of the file for skin	*/

		static const char* const FILE_FORMAT_SKINS;		/* the format of the skin file	*/

		struct PanelData
		{
			PanelType		type;
			PanelType		splitFrom;
			PanelSplitType	splitType;
			int			viewportIndex;
		};

		static bool fullscreen;
		static int	width;		/* width of editor window						*/
		static int	height;		/* height of editor window						*/
		static int	posX;		/* x position on desktop from top-left corner	*/
		static int	posY;		/* y position on desktop						*/
		static int	fps;		/* desired FPS when rendering					*/

		static String projectName;
		static String projectPath;	/* path to project .cyredproj file	*/	

		static DataArray<PanelData>	panels;
	};
}