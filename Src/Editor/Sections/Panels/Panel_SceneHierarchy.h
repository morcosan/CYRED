// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "CyredModule_Event.h"
#include "CyredBuildingBlocks.h"

#include "../Panel.h"


class QMenu;
class QTreeWidgetItem;

namespace CYRED
{
	class CustomTreeItem;
	class Menu_GameObject;
	class Menu_Scene;
}


namespace CYRED
{
	class Panel_SceneHierarchy : public Panel, public IEventListener
	{
		const char*	const	PANEL_TITLE			= "Scene Hierarchy";
		const Vector2		MIN_SIZE			= Vector2( 200, 200 );
		const Vector2		MAX_SIZE			= Vector2( 300, 1000 );


	public:
		Panel_SceneHierarchy();
		virtual ~Panel_SceneHierarchy() {}


	public:
		void Initialize	() override;
		void Finalize	() override;
		void OnEvent	( EventType eType, void* eData )	override;


	public:
		void A_ItemClicked			( QTreeWidgetItem* item, int column );
		void A_ItemRenamed			( QTreeWidgetItem* item, int column );
		void A_RightClickMenu		( const QPoint& pos );


	private:
		class _QtTree;

		_QtTree*			_qtTree;
		Menu_GameObject*	_menuGameObject;
		Menu_Scene*			_menuScene;

		CustomTreeItem*		_FindGameObjectItem		( int uid );
		CustomTreeItem*		_FindSceneItem			( const char* uid );
		void				_CreateRightClickMenu	();
		void				_ResetHierarchy			();
		void				_RecResetHierarchy		( GameObject* gameObject, QTreeWidgetItem* parent );
	};
}