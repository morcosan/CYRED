// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredModule_Event.h"
#include "CyredBuildingBlocks.h"

#include "../Panel.h"


class QTreeWidgetItem;

namespace CYRED
{
	class CustomTreeItem;
	class Menu_GameObject;
	class Menu_Prefab;
	class Prefab;
}


namespace CYRED
{
	class Panel_PrefabHierarchy : public Panel, public IEventListener
	{
		const Char*	const	PANEL_TITLE			= "Prefab Hierarchy";
		const Vector2		MIN_SIZE			= Vector2( 200, 100 );
		const Vector2		MAX_SIZE			= Vector2( 300, 1000 );


	public:
		Panel_PrefabHierarchy();
		virtual ~Panel_PrefabHierarchy() {}


	public:
		void Initialize	() override;
		void Finalize	() override;
		void OnEvent	( EventType eType, void* eData )	override;


	public:
		void A_ItemClicked			( QTreeWidgetItem* item, int column );
		void A_ItemRenamed			( QTreeWidgetItem* item, int column );
		void A_RightClickMenu		( const QPoint& pos );


	protected:
		class _QtTree;

		_QtTree*			_qtTree;
		Menu_GameObject*	_menuGameObject;
		Menu_Prefab*		_menuPrefab;

		CustomTreeItem*		_FindGameObjectItem		( UInt uid );
		CustomTreeItem*		_FindPrefabItem			( const Char* uid );
		void				_CreateRightClickMenu	();
		void				_ResetHierarchy			( Prefab* prefab );
		void				_RecResetHierarchy		( GameObject* gameObject, QTreeWidgetItem* parent );
	};
}