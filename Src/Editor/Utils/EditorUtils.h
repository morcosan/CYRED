// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"


namespace CYRED
{
	class Asset;
}


namespace CYRED
{
	class EditorUtils
	{
	public:
		static const char* const	ICON_SCENE;
		static const char* const	ICON_FOLDER;
		static const char* const	ICON_MATERIAL;
		static const char* const	ICON_MESH;
		static const char* const	ICON_MORPH;
		static const char* const	ICON_TEXTURE;
		static const char* const	ICON_SHADER;
		static const char* const	ICON_SCRIPT;
		static const char* const	ICON_PREFAB;
		static const char* const	ICON_UNKNOWN;
		
		static const char* const	NAME_FOLDER;
		static const char* const	NAME_MATERIAL;
		static const char* const	NAME_MAT_EMPTY;
		static const char* const	NAME_MAT_PS;
		static const char* const	NAME_TEXTURE;
		static const char* const	NAME_TEX_2D;
		static const char* const	NAME_TEX_CM;
		static const char* const	NAME_SHADER;
		static const char* const	NAME_MESH;
		static const char* const	NAME_MORPH;
		static const char* const	NAME_SCRIPT;
		static const char* const	NAME_SCENE;


	public:
		static void SetAvailableName	( Asset* asset );
		static bool IsFilePathDuplicate	( Asset* asset );


	private:
		EditorUtils() {}
		~EditorUtils() {}
	};
}