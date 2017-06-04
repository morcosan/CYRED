// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredBuildingBlocks.h"

#include "../../Utils/EditorEvents.h"
#include "../Panel.h"

class QMenu;
class QTreeWidgetItem;

namespace CYRED
{
	class CustomTreeItem;
	class CustomTree;
}


namespace CYRED
{
	ABSTRACT class Panel_Hierarchy : public Panel, public IEventListener
	{
		const Vector2	MIN_SIZE = Vector2( 150, 200 );


	public:
		Panel_Hierarchy() {}
		virtual ~Panel_Hierarchy() {}


	public:
		void Initialize	()	override;
		void Finalize	()	override;


	public:
		virtual void A_ItemClicked		( QTreeWidgetItem* item, int column )	PURE_VIRTUAL;
		virtual void A_RightClickMenu	( const QPoint& pos )					PURE_VIRTUAL;
		virtual void OnAction_Isolate	( GameObject* gameObject )				PURE_VIRTUAL;


	public:
		void UpdateNeedsSave	( bool needsSave );

		void A_ItemRenamed		( QTreeWidgetItem* item, int column );


	protected:
		virtual cchar*	_GetPanelTitle	() PURE_VIRTUAL;
		virtual void	_OnInitialize	() PURE_VIRTUAL;


	protected:
		CustomTree* _qtTree;
		bool		_currNeedsSave;


	protected:
		void _RecResetHierarchy	( Node* node, QTreeWidgetItem* parentItem );
	};
}