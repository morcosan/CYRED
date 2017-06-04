// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredBuildingBlocks.h"

#include "../Panel_Hierarchy.h"

class QMenu;
class QTreeWidgetItem;

namespace CYRED
{
	class Menu_GameObject;
	class Menu_Scene;
}


namespace CYRED
{
	class Hierarchy_Scene : public Panel_Hierarchy
	{
		cchar* const PANEL_TITLE = "Scene Hierarchy";


	public:
		Hierarchy_Scene() {}
		virtual ~Hierarchy_Scene() {}


	public:
		void OnEvent			( int eventType, void* eventData )		override;

		void A_ItemClicked		( QTreeWidgetItem* item, int column )	override;
		void A_RightClickMenu	( const QPoint& pos )					override;
		void OnAction_Isolate	( GameObject* gameObject )				override;


	protected:
		cchar*	_GetPanelTitle	()	override;
		void	_OnInitialize	()	override;


	private:
		Menu_GameObject*	_menuGameObject;
		Menu_Scene*			_menuScene;
		bool				_playModeNeedsSave;


	private:
		CustomTreeItem*	_FindGameObjectItem		( int uid );
		CustomTreeItem*	_FindSceneItem			( cchar* uid );
		void			_ResetHierarchy			();
	};
}