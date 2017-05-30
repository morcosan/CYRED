// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"



namespace CYRED
{
	struct EditorSkin
	{
		static cchar* const ATTR_GROUP;				/* group of attribute panel			*/
		static cchar* const ATTR_GROUP_TREE;		
		static cchar* const ATTR_BUTTON;			/* button of attribute panel		*/
		static cchar* const ATTR_SELECTOR_BUTTON;	/* selector button from attr panel	*/
		static cchar* const ATTR_COMP_TREE;			/* component tree of attr panel		*/
		static cchar* const ATTR_COMP_TITLE;		/* component title of attr panel	*/
		static cchar* const ATTR_COMP_SETTINGS;		

		static cchar* const SELECTOR_POPUP;			/* selector window					*/

		static cchar* const HIERARCHY_TREE;			/* the QTree of hierarchy panel			*/
		static cchar* const HIERARCHY_TREE_SAVE;	
		
		static cchar* const ASSET_TREE;
		static cchar* const ASSET_BUTTON;
		static cchar* const ASSET_UNKNOWN;	

		static cchar* const VIEWPORT_BUTTON;		/* a button of viewport panel			*/
		static cchar* const VIEWPORT_VIEW;
		
		static cchar* const PANEL_LAYOUT;			/* the layout inside a panel			*/
		
		static cchar* const CONSOLE_LAYOUT;			/* the layout of the console			*/
		static cchar* const CONSOLE_LOG;			/* the log inside the console			*/

		static cchar* const FLAG_DISABLED;			/* when an item is disabled				*/
	};
}