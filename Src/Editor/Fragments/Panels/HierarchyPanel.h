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
	class HierarchyPanel : public Panel, public EventListener
	{
		const Char*	const	PANEL_TITLE			= "Hierarchy Panel";
		const Vector2		MIN_SIZE			= Vector2( 200, 250 );

		const Char* const	MSG_SAVE_SCENE		= "Save Scene";
		const Char* const	FILE_FILTER_SCENE	= "Scene (*%s)";

		const Char* const	MENU_SAVE_SCENE		= "Save Scene";
		const Char* const	MENU_SAVE_SCENE_AS	= "Save Scene As..";
		const Char* const	MENU_CLOSE_SCENE	= "Close Scene";
		const Char* const	MENU_RENAME			= "Rename";
		const Char* const	MENU_DELETE			= "Delete";

		const Char*	const	MENU_GO				= "Create Object";
		const Char*	const	MENU_GO_EMPTY		= "Empty";
		const Char*	const	MENU_GO_3D			= "3D";
		const Char*	const	MENU_GO_3D_PIVOT	= "Pivot";
		const Char*	const	MENU_GO_3D_CAMERA	= "Camera";
		const Char*	const	MENU_GO_3D_MESH		= "Mesh";
		const Char*	const	MENU_GO_3D_MORPH	= "Morph";
		const Char*	const	MENU_GO_PS			= "Particles";
		const Char*	const	MENU_GO_PS_EMITTER	= "Emitter";

		const Char* const	STATUS_NEW_GO		= "New GameObject created.";


	public:
		HierarchyPanel();
		virtual ~HierarchyPanel() {}


	public:
		void Initialize	()	override;

		void OnEvent	( EventType eType, EventName eName, void* eSource )	override;


	public:
		void A_ItemClicked			( QTreeWidgetItem* item, int column );
		void A_ItemRenamed			( QTreeWidgetItem* item, int column );
		void A_RightClickMenu		( const QPoint& pos );

		void A_SaveScene			();
		void A_SaveSceneAs			();
		void A_CloseScene			();
		void A_Rename				();
		void A_Delete				();

		void A_GO_CreateEmpty		();
		void A_GO_Create3D_Pivot	();
		void A_GO_Create3D_Camera	();
		void A_GO_Create3D_Mesh		();
		void A_GO_Create3D_Morph	();
		void A_GO_Particles_Emitter	();


	protected:
		class _QtTree;
		class _QtTreeItem;

		_QtTree*	_qtTree;
		QMenu*		_qtRightClickMenu;

		void			_RecResetHierarchy	( GameObject* gameObject, QTreeWidgetItem* parent );
		_QtTreeItem*	_FindGameObjectItem	( UInt uid );
		_QtTreeItem*	_FindSceneItem		( const Char* uid );

		void _CreateRightClickMenu	();
		void _AddRightClickActions	( QTreeWidgetItem* item );
		
		/* each time something changes, reset all hierarchy */
		void _ResetHierarchy		();
	};
}