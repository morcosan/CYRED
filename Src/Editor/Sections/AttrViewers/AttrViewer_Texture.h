// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"


namespace CYRED
{
	class Texture;
}


namespace CYRED
{
	class AttrViewer_Texture : public AttrViewer
	{
	public:
		const char* TITLE					= "Texture";

		const char* ATTR_NAME				= "Name";
		const char* ATTR_TEXTURE_TYPE		= "Texture Type";
		const char* ATTR_LOAD_TYPE			= "Load Type";
		const char* TYPE_TEXTURE_2D			= "TEXTURE_2D";
		const char* TYPE_CUBE_MAP			= "CUBE_MAP";
		const char* TYPE_SCRIPTED			= "SCRIPTED";
		const char* TYPE_EXTERNAL			= "EXTERNAL";
		const char* ATTR_HAS_MIPMAP			= "Has Mipmap";
		const char* ATTR_CLEAR_BUFFER		= "Clear Buffer On Bind";
		const char* ATTR_FILE_PATH			= "File Path";
		const char* ATTR_FILE_PATH_POSX		= "File Path PosX";
		const char* ATTR_FILE_PATH_NEGX		= "File Path NegX";
		const char* ATTR_FILE_PATH_POSY		= "File Path PosY";
		const char* ATTR_FILE_PATH_NEGY		= "File Path NegY";
		const char* ATTR_FILE_PATH_POSZ		= "File Path PosZ";
		const char* ATTR_FILE_PATH_NEGZ		= "File Path NegZ";


	public:
		AttrViewer_Texture() {}
		virtual ~AttrViewer_Texture() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		Texture*	_target;

		void _ChangeVisibility	();
	};
}