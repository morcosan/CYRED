// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredModule_Event.h"
#include "CyredBuildingBlocks.h"

#include <QtWidgets/QMenuBar>


namespace CYRED
{
	namespace Enums
	{
		enum _MenuAction
		{
	
		};
	}
	typedef Enums::_MenuAction	MenuAction;
}


namespace CYRED
{
	class MenuBar : public QMenuBar
	{
	public:
		const char*	const SCENE_MENU				= "Scene";
		const char*	const SCENE_OPEN_NEW			= "Open New";
		const char*	const SCENE_LOAD_NEW			= "Load New";
		const char*	const SCENE_SAVE_ALL			= "Save All";
		const char*	const SCENE_CLOSE_ALL			= "Close All";

		const char*	const PROJECT_MENU				= "Project";
		const char*	const PROJECT_SETTINGS			= "Settings";
		const char*	const PROJECT_BUILD_WIN			= "Build Windows";

		const char* const STATUS_OPEN_NEW_SCENE		= "New scene opened.";
		const char* const STATUS_LOAD_NEW_SCENE		= "New scene loaded.";
		const char* const STATUS_CLOSE_ALL_SCENES	= "All scenes closed.";

		const char* const SKINS_MENU				= "Skins";


	public:
		MenuBar() {}
		virtual ~MenuBar() {}


	public:
		void Initialize();
		void AddSkins( const DataArray<const char*>& skinsName, int defaultIndex );

		void A_Scene_OpenNew	();
		void A_Scene_LoadNew	();
		void A_Scene_SaveAll	();
		void A_Scene_CloseAll	();

		void A_Project_Settings	();
		void A_Project_BuildWin	();

		void A_Skins			( QAction* action );

		// TODO
		void A_Magic();


	protected:
		DataMap<MenuAction, QAction*> _menuActions;		/* only the default actions are stored */

		void _AddMenu_Scene		();
		void _AddMenu_Project	();
	};
}