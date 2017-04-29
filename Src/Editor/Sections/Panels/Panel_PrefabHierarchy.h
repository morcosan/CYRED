// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredModule_Event.h"
#include "CyredBuildingBlocks.h"

#include "../Panel.h"


class QTreeWidgetItem;
class QMenu;


namespace CYRED
{
	class CustomTreeItem;
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

		_QtTree*	_qtTree;
		QMenu*		_qtRightClickMenu;

		void			_RecResetHierarchy	( GameObject* gameObject, QTreeWidgetItem* parent );
		CustomTreeItem*	_FindGameObjectItem	( UInt uid );

		void _CreateRightClickMenu	();

		/* each time something changes, reset all hierarchy */
		void _ResetHierarchy		();
	};
}