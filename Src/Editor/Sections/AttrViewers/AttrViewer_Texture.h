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
		cchar* const TITLE					= "Texture";

		cchar* const ATTR_NAME				= "Name";
		cchar* const ATTR_TEXTURE_TYPE		= "Texture Type";
		cchar* const ATTR_LOAD_TYPE			= "Load Type";
		cchar* const TYPE_TEXTURE_2D		= "TEXTURE_2D";
		cchar* const TYPE_CUBE_MAP			= "CUBE_MAP";
		cchar* const TYPE_SCRIPTED			= "SCRIPTED";
		cchar* const TYPE_EXTERNAL			= "EXTERNAL";
		cchar* const ATTR_HAS_MIPMAP		= "Has Mipmap";
		cchar* const ATTR_CLEAR_BUFFER		= "Clear Buffer On Bind";
		cchar* const ATTR_FILE_PATH			= "File Path";
		cchar* const ATTR_FILE_PATH_POSX	= "File Path PosX";
		cchar* const ATTR_FILE_PATH_NEGX	= "File Path NegX";
		cchar* const ATTR_FILE_PATH_POSY	= "File Path PosY";
		cchar* const ATTR_FILE_PATH_NEGY	= "File Path NegY";
		cchar* const ATTR_FILE_PATH_POSZ	= "File Path PosZ";
		cchar* const ATTR_FILE_PATH_NEGZ	= "File Path NegZ";


	public:
		AttrViewer_Texture() {}
		virtual ~AttrViewer_Texture() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _UpdateGUI		()					override;
		void _UpdateTarget	()					override;


	private:
		Texture*	_target;


	private:
		void _ChangeVisibility	();
	};
}