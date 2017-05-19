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
}


namespace CYRED
{
	class Menu_GameObject : public QMenu
	{
		const char* const	MENU_RENAME				= "Rename";
		const char* const	MENU_DUPLICATE			= "Duplicate";
		const char* const	MENU_DELETE				= "Delete";
	
		const char* const	MSG_SAVE_PREFAB			= "Save Prefab";
		const char* const	FILE_FILTER_PREFAB		= "Prefab (*%s)";
	
		const char* const	MENU_CREATE_PREFAB		= "Create Prefab";
	
		const char*	const	MENU_GO					= "Create Child";
		const char*	const	MENU_GO_EMPTY			= "Empty";
		const char*	const	MENU_GO_3D				= "3D";
		const char*	const	MENU_GO_3D_PIVOT		= "Pivot";
		const char*	const	MENU_GO_3D_CAMERA		= "Camera";
		const char*	const	MENU_GO_3D_LIGHT		= "Light";
		const char*	const	MENU_GO_3D_MESH			= "Mesh";
		const char*	const	MENU_GO_3D_MORPH		= "Morph";
		const char*	const	MENU_GO_PS				= "Particles";
		const char*	const	MENU_GO_PS_EMITTER		= "Emitter";

		const char* const	MENU_ADD_COMPONENT		= "Add Component";
		const char* const	MENU_COMP_CAMERA		= "Camera";
		const char* const	MENU_COMP_LIGHT			= "Light";
		const char* const	MENU_COMP_MESH_REN		= "Mesh Rendering";
		const char* const	MENU_COMP_MORPH_REN		= "Morph Rendering";
		const char* const	MENU_COMP_PS_EMITTER	= "Particles Emitter";
		const char* const	MENU_COMP_SCRIPTER		= "Scripter";


	public:
		Menu_GameObject( QTreeWidget* qtTree, EventType eventType );
		virtual ~Menu_GameObject() {}


	public:
		void Open( const QPoint& pos );

		
	public:
		void A_Rename					();
		void A_Duplicate				();
		void A_Delete					();
	
		void A_CreatePrefab				();
	
		void A_GO_CreateEmpty			();
		void A_GO_Create3D_Pivot		();
		void A_GO_Create3D_Camera		();
		void A_GO_Create3D_Light		();
		void A_GO_Create3D_Mesh			();
		void A_GO_Create3D_Morph		();
		void A_GO_Particles_Emitter		();

		void A_AddComp_Camera			();
		void A_AddComp_Light			();
		void A_AddComp_MeshRendering	();
		void A_AddComp_MorphRendering	();
		void A_AddComp_ParticlesEmitter	();
		void A_AddComp_Scripter			();


	private:
		QTreeWidget*	_qtTree;
		EventType		_eventType;


	protected:
		GameObject*		_CreateGameObject	( Node* parentNode );
		void			_DestroyGameObject	( GameObject* gameObject );
	};
}