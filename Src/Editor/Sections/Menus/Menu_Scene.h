// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"

#include "QtWidgets\qmenu.h"


class QTreeWidget;


namespace CYRED
{
	class Menu_Scene : public QMenu
	{
		const Char* const	MSG_SAVE_SCENE		= "Save Scene";
		const Char* const	FILE_FILTER_SCENE	= "Scene (*%s)";

		const Char* const	MENU_SAVE_SCENE		= "Save Scene";
		const Char* const	MENU_SAVE_SCENE_AS	= "Save Scene As..";
		const Char* const	MENU_CLOSE_SCENE	= "Close Scene";

		const Char* const	MENU_RENAME			= "Rename";

		const Char*	const	MENU_GO				= "Create Object";
		const Char*	const	MENU_GO_EMPTY		= "Empty";
		const Char*	const	MENU_GO_3D			= "3D";
		const Char*	const	MENU_GO_3D_PIVOT	= "Pivot";
		const Char*	const	MENU_GO_3D_CAMERA	= "Camera";
		const Char*	const	MENU_GO_3D_LIGHT	= "Light";
		const Char*	const	MENU_GO_3D_MESH		= "Mesh";
		const Char*	const	MENU_GO_3D_MORPH	= "Morph";
		const Char*	const	MENU_GO_PS			= "Particles";
		const Char*	const	MENU_GO_PS_EMITTER	= "Emitter";


	public:
		Menu_Scene( QTreeWidget* qtTree );
		virtual ~Menu_Scene() {}


	public:
		void Open( const QPoint& pos );


	public:
		void A_SaveScene				();
		void A_SaveSceneAs				();
		void A_CloseScene				();
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
	};
}