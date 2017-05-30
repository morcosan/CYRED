// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
			, NEW_TAB
		};
	}
	typedef Enum_PanelSplitType::Enum	PanelSplitType;
}


namespace CYRED
{
	struct EditorSettings
	{
		static cchar* const EDITOR_NAME;			/* main title of editor			*/

		static cchar* const FILE_PATH_CONFIG;		

		static cchar* const DIR_PATH_SKINS;		/* directory for skins			*/
		static cchar* const DIR_PATH_ICONS;		

		static cchar* const DIR_DEFALUT_ASSETS;		
		static cchar* const DIR_PATH_DEFALUT_ASSETS;		

		static cchar* const MSG_EDITOR_STARTED;	/* message when editor starts	*/

		static cchar* const SKIN_DEFAULT_BLACK;	/* name of the file for skin	*/
		static cchar* const SKIN_DEFAULT_WHITE;	/* name of the file for skin	*/

		static cchar* const FILE_FORMAT_SKINS;		/* the format of the skin file	*/

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