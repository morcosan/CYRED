// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredBuildingBlocks.h"

class QIcon;


namespace CYRED
{
	class EditorUtils
	{
	public:
		static cchar* const	ICON_SCENE;
		static cchar* const	ICON_FOLDER;
		static cchar* const	ICON_MATERIAL;
		static cchar* const	ICON_MESH;
		static cchar* const	ICON_MORPH;
		static cchar* const	ICON_TEXTURE;
		static cchar* const	ICON_SHADER;
		static cchar* const	ICON_SCRIPT;
		static cchar* const	ICON_PREFAB;
		static cchar* const	ICON_UNKNOWN;
		static cchar* const	ICON_LOG_INFO;
		static cchar* const	ICON_LOG_ERROR;
		static cchar* const	ICON_SETTINGS;
		
		static cchar* const	NAME_FOLDER;
		static cchar* const	NAME_MATERIAL;
		static cchar* const	NAME_MAT_EMPTY;
		static cchar* const	NAME_MAT_PS;
		static cchar* const	NAME_TEXTURE;
		static cchar* const	NAME_TEX_2D;
		static cchar* const	NAME_TEX_CM;
		static cchar* const	NAME_SHADER;
		static cchar* const	NAME_MESH;
		static cchar* const	NAME_MORPH;
		static cchar* const	NAME_SCRIPT;
		static cchar* const	NAME_SCENE;
		static cchar* const	NAME_PREFAB;


	public:
		static void		Initialize			();
		static void		SetAvailableName	( Asset* asset );
		static bool		IsFilePathDuplicate	( Asset* asset );
		static QIcon*	GetIcon				( cchar* iconName );


	private:
		static DataMap<String, QIcon*>	_icons;


	private:
		EditorUtils() {}
		~EditorUtils() {}
	};
}