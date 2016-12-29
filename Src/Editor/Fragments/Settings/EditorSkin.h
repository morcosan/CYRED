// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"



namespace CYRED
{
	struct EditorSkin
	{
		static const Char* const ATTR_GROUP;			/* group of attribute panel			*/
		static const Char* const ATTR_GROUP_TREE;		
		static const Char* const ATTR_BUTTON;			/* button of attribute panel		*/
		static const Char* const ATTR_SELECTOR_BUTTON;	/* selector button from attr panel	*/
		static const Char* const ATTR_COMP_TREE;		/* component tree of attr panel		*/
		static const Char* const ATTR_COMP_TITLE;		/* component title of attr panel	*/

		static const Char* const SELECTOR_POPUP;		/* selector window					*/

		static const Char* const HIERARCHY_TREE;	/* the QTree of hierarchy panel			*/
		
		static const Char* const ASSET_TREE;
		static const Char* const ASSET_BUTTON;
		static const Char* const ASSET_UNKNOWN;	

		static const Char* const VIEWPORT_BUTTON;	/* a button of viewport panel			*/

		static const Char* const PANEL_LAYOUT;		/* the layout inside a panel			*/
		
		static const Char* const CONSOLE_LAYOUT;	/* the layout of the console			*/
		static const Char* const CONSOLE_LOG;		/* the log inside the console			*/

		static const Char* const FLAG_DISABLED;		/* when an item is disabled				*/
	};
}