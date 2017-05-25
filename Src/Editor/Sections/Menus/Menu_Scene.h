// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"

#include "QtWidgets\qmenu.h"


class QTreeWidget;


namespace CYRED
{
	class Menu_Scene : public QMenu
	{
		cchar* const	MSG_SAVE_SCENE		= "Save Scene";
		cchar* const	FILE_FILTER_SCENE	= "Scene (*%s)";

		cchar* const	MENU_SAVE_SCENE		= "Save Scene";
		cchar* const	MENU_SAVE_SCENE_AS	= "Save Scene As..";
		cchar* const	MENU_CLOSE_SCENE	= "Close Scene";

		cchar* const	MENU_RENAME			= "Rename";

		cchar*	const	MENU_GO				= "Create Object";
		cchar*	const	MENU_GO_EMPTY		= "Empty";
		cchar*	const	MENU_GO_3D			= "3D";
		cchar*	const	MENU_GO_3D_PIVOT	= "Pivot";
		cchar*	const	MENU_GO_3D_CAMERA	= "Camera";
		cchar*	const	MENU_GO_3D_LIGHT	= "Light";
		cchar*	const	MENU_GO_3D_MESH		= "Mesh";
		cchar*	const	MENU_GO_3D_MORPH	= "Morph";
		cchar*	const	MENU_GO_PS			= "Particles";
		cchar*	const	MENU_GO_PS_EMITTER	= "Emitter";


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