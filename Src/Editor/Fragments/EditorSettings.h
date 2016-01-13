// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "CyredBuildingBlocks.h"



namespace CYRED
{
	struct EditorSettings
	{
		static const Char* const EDITOR_NAME;			/* main title of editor			*/

		static const Char* const DIR_PATH_SKINS;		/* directory for skins			*/
		static const Char* const DIR_PATH_ICONS_ASSETS;		

		static const Char* const DIR_PATH_ASSETS;		

		static const Char* const MSG_EDITOR_STARTED;	/* message when editor starts	*/

		static const Char* const SKIN_DEFAULT_BLACK;	/* name of the file for skin	*/
		static const Char* const SKIN_DEFAULT_WHITE;	/* name of the file for skin	*/

		static const Char* const FILE_FORMAT_SKINS;		/* the format of the skin file	*/


		static UInt	width;		/* width of editor window						*/
		static UInt	height;		/* height of editor window						*/
		static Int	posX;		/* x position on desktop from top-left corner	*/
		static Int	posY;		/* y position on desktop						*/
		static UInt	fps;		/* desired FPS when rendering					*/

		static String dirPathProject;	/* path to project directory */	
	};
}