// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredModule_Event.h"

#include "QtWidgets\qmenu.h"

class QTreeWidget;

namespace CYRED
{
	class GameObject;
	class Node;
	class Panel_Hierarchy;
}


namespace CYRED
{
	class Menu_GameObject : public QMenu
	{
		cchar* const	MENU_RENAME				= "Rename";
		cchar* const	MENU_DUPLICATE			= "Duplicate";
		cchar* const	MENU_DELETE				= "Delete";

		cchar* const	MENU_SAVE_PREFAB		= "Save Prefab";
		cchar* const	MENU_SAVE_PREFAB_AS		= "Save Prefab As..";
		cchar* const	MENU_CLOSE_PREFAB		= "Close Prefab";
	
		cchar* const	MSG_SAVE_PREFAB			= "Save Prefab";
		cchar* const	FILE_FILTER_PREFAB		= "Prefab (*%s)";
	
		cchar* const	MENU_ISOLATE			= "Isolate";
		cchar* const	MENU_CREATE_PREFAB		= "Create Prefab";
	
		cchar*	const	MENU_GO					= "Create Child";
		cchar*	const	MENU_GO_EMPTY			= "Empty";
		cchar*	const	MENU_GO_3D				= "3D";
		cchar*	const	MENU_GO_3D_PIVOT		= "Pivot";
		cchar*	const	MENU_GO_3D_CAMERA		= "Camera";
		cchar*	const	MENU_GO_3D_LIGHT		= "Light";
		cchar*	const	MENU_GO_3D_MESH			= "Mesh";
		cchar*	const	MENU_GO_3D_MORPH		= "Morph";
		cchar*	const	MENU_GO_PS				= "Particles";
		cchar*	const	MENU_GO_PS_EMITTER		= "Emitter";
		cchar*	const	MENU_GO_TEXT_3D			= "Text 3D";

		cchar* const	MENU_ADD_COMPONENT		= "Add Component";
		cchar* const	MENU_COMP_TRANSFORM		= "Transform";
		cchar* const	MENU_COMP_CAMERA		= "Camera";
		cchar* const	MENU_COMP_LIGHT			= "Light";
		cchar* const	MENU_COMP_MESH_REN		= "Mesh Rendering";
		cchar* const	MENU_COMP_MORPH_REN		= "Morph Rendering";
		cchar* const	MENU_COMP_PS_EMITTER	= "Particles Emitter";
		cchar* const	MENU_COMP_SCRIPTER		= "Scripter";
		cchar* const	MENU_COMP_RIGID_BODY	= "Rigid Body";
		cchar* const	MENU_COMP_TEXT_3D		= "Text 3D";


	public:
		Menu_GameObject( QTreeWidget* qtTree, Panel_Hierarchy* panel );
		virtual ~Menu_GameObject() {}


	public:
		void Open( const QPoint& pos, bool isPrefab );

		
	public:
		void A_SavePrefab				();
		void A_SavePrefabAs				();
		void A_ClosePrefab				();

		void A_Rename					();
		void A_Duplicate				();
		void A_Delete					();
	
		void A_Isolate					();
		void A_CreatePrefab				();
	
		void A_GO_CreateEmpty			();
		void A_GO_Create3D_Pivot		();
		void A_GO_Create3D_Camera		();
		void A_GO_Create3D_Light		();
		void A_GO_Create3D_Mesh			();
		void A_GO_Create3D_Morph		();
		void A_GO_Particles_Emitter		();
		void A_GO_Text_3D				();

		void A_AddComp_Transform		();
		void A_AddComp_Camera			();
		void A_AddComp_Light			();
		void A_AddComp_MeshRendering	();
		void A_AddComp_MorphRendering	();
		void A_AddComp_ParticlesEmitter	();
		void A_AddComp_Scripter			();
		void A_AddComp_RigidBody		();
		void A_AddComp_Text3D			();


	private:
		QTreeWidget*		_qtTree;
		Panel_Hierarchy*	_panel;


	private:
		GameObject*		_CreateGameObject	( Node* parentNode );
	};
}