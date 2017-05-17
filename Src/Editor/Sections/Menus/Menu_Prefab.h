// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"

#include "QtWidgets\qmenu.h"


class QTreeWidget;

namespace CYRED
{
	class Node;
	class GameObject;
}


namespace CYRED
{
	class Menu_Prefab : public QMenu
	{
		const char* const	MSG_SAVE_PREFAB		= "Save Prefab";
		const char* const	FILE_FILTER_PREFAB	= "Prefab (*%s)";

		const char* const	MENU_SAVE_PREFAB	= "Save Prefab";
		const char* const	MENU_SAVE_PREFAB_AS	= "Save Prefab As..";
		const char* const	MENU_CLOSE_PREFAB	= "Close Prefab";

		const char* const	MENU_RENAME			= "Rename";

		const char*	const	MENU_GO				= "Create Object";
		const char*	const	MENU_GO_EMPTY		= "Empty";
		const char*	const	MENU_GO_3D			= "3D";
		const char*	const	MENU_GO_3D_PIVOT	= "Pivot";
		const char*	const	MENU_GO_3D_CAMERA	= "Camera";
		const char*	const	MENU_GO_3D_LIGHT	= "Light";
		const char*	const	MENU_GO_3D_MESH		= "Mesh";
		const char*	const	MENU_GO_3D_MORPH	= "Morph";
		const char*	const	MENU_GO_PS			= "Particles";
		const char*	const	MENU_GO_PS_EMITTER	= "Emitter";


	public:
		Menu_Prefab( QTreeWidget* qtTree );
		virtual ~Menu_Prefab() {}


	public:
		void Open( const QPoint& pos );


	public:
		void A_SavePrefab				();
		void A_SavePrefabAs				();
		void A_ClosePrefab				();

		void A_Rename					();

		void A_GO_CreateEmpty			();
		void A_GO_Create3D_Pivot		();
		void A_GO_Create3D_Camera		();
		void A_GO_Create3D_Light		();
		void A_GO_Create3D_Mesh			();
		void A_GO_Create3D_Morph		();
		void A_GO_Particles_Emitter		();


	private:
		QTreeWidget*	_qtTree;


	private:
		GameObject*		_CreateGameObject( Node* parentNode );
	};
}