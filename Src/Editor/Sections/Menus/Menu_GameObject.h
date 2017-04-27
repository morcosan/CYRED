// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"

#include "QtWidgets\qmenu.h"


class QTreeWidget;


namespace CYRED
{
	class Menu_GameObject : public QMenu
	{
		const Char* const	MENU_RENAME				= "Rename";
		const Char* const	MENU_DUPLICATE			= "Duplicate";
		const Char* const	MENU_DELETE				= "Delete";
	
		const Char* const	MSG_SAVE_PREFAB			= "Save Prefab";
		const Char* const	FILE_FILTER_PREFAB		= "Prefab (*%s)";
	
		const Char* const	MENU_CREATE_PREFAB		= "Create Prefab";
	
		const Char*	const	MENU_GO					= "Create Child";
		const Char*	const	MENU_GO_EMPTY			= "Empty";
		const Char*	const	MENU_GO_3D				= "3D";
		const Char*	const	MENU_GO_3D_PIVOT		= "Pivot";
		const Char*	const	MENU_GO_3D_CAMERA		= "Camera";
		const Char*	const	MENU_GO_3D_LIGHT		= "Light";
		const Char*	const	MENU_GO_3D_MESH			= "Mesh";
		const Char*	const	MENU_GO_3D_MORPH		= "Morph";
		const Char*	const	MENU_GO_PS				= "Particles";
		const Char*	const	MENU_GO_PS_EMITTER		= "Emitter";

		const Char* const	MENU_ADD_COMPONENT		= "Add Component";
		const Char* const	MENU_COMP_CAMERA		= "Camera";
		const Char* const	MENU_COMP_LIGHT			= "Light";
		const Char* const	MENU_COMP_MESH_REN		= "Mesh Rendering";
		const Char* const	MENU_COMP_MORPH_REN		= "Morph Rendering";
		const Char* const	MENU_COMP_PS_EMITTER	= "Particles Emitter";
		const Char* const	MENU_COMP_SCRIPTER		= "Scripter";


	public:
		Menu_GameObject( QTreeWidget* qtTree );
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
	};
}