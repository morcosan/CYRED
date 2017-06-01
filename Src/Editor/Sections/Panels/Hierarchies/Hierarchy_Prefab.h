// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredBuildingBlocks.h"

#include "../Panel_Hierarchy.h"

class QTreeWidgetItem;

namespace CYRED
{
	class Menu_GameObject;
	class Prefab;
}


namespace CYRED
{
	class Hierarchy_Prefab : public Panel_Hierarchy
	{
		cchar* const PANEL_TITLE = "Prefab Hierarchy";


	public:
		Hierarchy_Prefab() {}
		virtual ~Hierarchy_Prefab() {}


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
		Prefab*				_targetPrefab;


	private:
		CustomTreeItem*		_FindGameObjectItem		( int uid );
		CustomTreeItem*		_FindPrefabItem			( cchar* uid );
		void				_ResetHierarchy			();
	};
}