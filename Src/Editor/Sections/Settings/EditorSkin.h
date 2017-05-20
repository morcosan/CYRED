// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"



namespace CYRED
{
	struct EditorSkin
	{
		static const char* const ATTR_GROUP;			/* group of attribute panel			*/
		static const char* const ATTR_GROUP_TREE;		
		static const char* const ATTR_BUTTON;			/* button of attribute panel		*/
		static const char* const ATTR_SELECTOR_BUTTON;	/* selector button from attr panel	*/
		static const char* const ATTR_COMP_TREE;		/* component tree of attr panel		*/
		static const char* const ATTR_COMP_TITLE;		/* component title of attr panel	*/

		static const char* const SELECTOR_POPUP;		/* selector window					*/

		static const char* const HIERARCHY_TREE;	/* the QTree of hierarchy panel			*/
		
		static const char* const ASSET_TREE;
		static const char* const ASSET_BUTTON;
		static const char* const ASSET_UNKNOWN;	

		static const char* const VIEWPORT_BUTTON;	/* a button of viewport panel			*/
		static const char* const VIEWPORT_VIEW;
		
		static const char* const PANEL_LAYOUT;		/* the layout inside a panel			*/
		
		static const char* const CONSOLE_LAYOUT;	/* the layout of the console			*/
		static const char* const CONSOLE_LOG;		/* the log inside the console			*/

		static const char* const FLAG_DISABLED;		/* when an item is disabled				*/
	};
}