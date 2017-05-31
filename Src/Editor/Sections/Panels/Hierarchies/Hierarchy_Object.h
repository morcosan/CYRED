// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredModule_Event.h"
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
	class Hierarchy_Object : public Panel_Hierarchy, public IEventListener
	{
		cchar* const PANEL_TITLE = "Object Hierarchy";


	public:
		Hierarchy_Object() {}
		virtual ~Hierarchy_Object() {}


	public:
		void Finalize			()										override;
		void OnEvent			( int eventType, void* eventData )		override;

		void A_ItemClicked		( QTreeWidgetItem* item, int column )	override;
		void A_RightClickMenu	( const QPoint& pos )					override;


	protected:
		cchar*	_GetPanelTitle	()	override;
		void	_OnInitialize	()	override;


	private:
		Menu_GameObject*	_menuGameObject;
		GameObject*			_target;


	private:
		CustomTreeItem*		_FindGameObjectItem		( int uid );
		void				_CreateRightClickMenu	();
		void				_ResetHierarchy			();
	};
}