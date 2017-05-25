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
		cchar* TITLE					= "Texture";

		cchar* ATTR_NAME				= "Name";
		cchar* ATTR_TEXTURE_TYPE		= "Texture Type";
		cchar* ATTR_LOAD_TYPE			= "Load Type";
		cchar* TYPE_TEXTURE_2D			= "TEXTURE_2D";
		cchar* TYPE_CUBE_MAP			= "CUBE_MAP";
		cchar* TYPE_SCRIPTED			= "SCRIPTED";
		cchar* TYPE_EXTERNAL			= "EXTERNAL";
		cchar* ATTR_HAS_MIPMAP			= "Has Mipmap";
		cchar* ATTR_CLEAR_BUFFER		= "Clear Buffer On Bind";
		cchar* ATTR_FILE_PATH			= "File Path";
		cchar* ATTR_FILE_PATH_POSX		= "File Path PosX";
		cchar* ATTR_FILE_PATH_NEGX		= "File Path NegX";
		cchar* ATTR_FILE_PATH_POSY		= "File Path PosY";
		cchar* ATTR_FILE_PATH_NEGY		= "File Path NegY";
		cchar* ATTR_FILE_PATH_POSZ		= "File Path PosZ";
		cchar* ATTR_FILE_PATH_NEGZ		= "File Path NegZ";


	public:
		AttrViewer_Texture() {}
		virtual ~AttrViewer_Texture() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	private:
		Texture*	_target;


	private:
		void _ChangeVisibility	();
	};
}