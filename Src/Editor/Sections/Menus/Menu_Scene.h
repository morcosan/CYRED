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
		const char* const	MSG_SAVE_SCENE		= "Save Scene";
		const char* const	FILE_FILTER_SCENE	= "Scene (*%s)";

		const char* const	MENU_SAVE_SCENE		= "Save Scene";
		const char* const	MENU_SAVE_SCENE_AS	= "Save Scene As..";
		const char* const	MENU_CLOSE_SCENE	= "Close Scene";

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