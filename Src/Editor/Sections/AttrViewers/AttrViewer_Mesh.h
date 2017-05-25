// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"


namespace CYRED
{
	class Mesh;
}


namespace CYRED
{
	class AttrViewer_Mesh : public AttrViewer
	{
	public:
		cchar* TITLE					= "Mesh";

		cchar* ATTR_NAME				= "Name";
		cchar* ATTR_MESH_TYPE			= "Mesh Type";
		cchar* MESH_TYPE_LINE			= "LINE";
		cchar* MESH_TYPE_POLYGON		= "POLYGON";
		cchar* ATTR_LOAD_TYPE			= "Load Type";
		cchar* LOAD_TYPE_EXTERNAL		= "EXTERNAL";
		cchar* LOAD_TYPE_SCRIPTED		= "SCRIPTED";
		cchar* ATTR_FILE_PATH			= "File Path";
		cchar* ATTR_CLEAR_BUFFER		= "Clear Buffers On Bind";


	public:
		AttrViewer_Mesh() {}
		virtual ~AttrViewer_Mesh() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _UpdateGUI		()					override;
		void _UpdateTarget	()					override;


	private:
		Mesh*	_target;
	};
}