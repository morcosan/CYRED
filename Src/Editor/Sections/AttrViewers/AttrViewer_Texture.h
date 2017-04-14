// Copyright (c) 2015 Morco (www.morco.ro)
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
		const Char* TITLE					= "Texture";

		const Char* ATTR_NAME				= "Name";
		const Char* ATTR_TEXTURE_TYPE		= "Texture Type";
		const Char* ATTR_LOAD_TYPE			= "Load Type";
		const Char* TYPE_TEXTURE_2D			= "TEXTURE_2D";
		const Char* TYPE_CUBE_MAP			= "CUBE_MAP";
		const Char* TYPE_SCRIPTED			= "SCRIPTED";
		const Char* TYPE_EXTERNAL			= "EXTERNAL";
		const Char* ATTR_HAS_MIPMAP			= "Has Mipmap";
		const Char* ATTR_CLEAR_BUFFER		= "Clear Buffer On Bind";
		const Char* ATTR_FILE_PATH			= "File Path";
		const Char* ATTR_FILE_PATH_POSX		= "File Path PosX";
		const Char* ATTR_FILE_PATH_NEGX		= "File Path NegX";
		const Char* ATTR_FILE_PATH_POSY		= "File Path PosY";
		const Char* ATTR_FILE_PATH_NEGY		= "File Path NegY";
		const Char* ATTR_FILE_PATH_POSZ		= "File Path PosZ";
		const Char* ATTR_FILE_PATH_NEGZ		= "File Path NegZ";


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