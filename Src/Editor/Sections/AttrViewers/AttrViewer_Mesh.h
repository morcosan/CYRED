// Copyright (c) 2015 Morco (www.morco.ro)
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
		const Char* TITLE					= "Mesh";

		const Char* ATTR_NAME				= "Name";
		const Char* ATTR_MESH_TYPE			= "Mesh Type";
		const Char* MESH_TYPE_LINE			= "LINE";
		const Char* MESH_TYPE_POLYGON		= "POLYGON";
		const Char* ATTR_LOAD_TYPE			= "Load Type";
		const Char* LOAD_TYPE_EXTERNAL		= "EXTERNAL";
		const Char* LOAD_TYPE_SCRIPTED		= "SCRIPTED";
		const Char* ATTR_FILE_PATH			= "File Path";
		const Char* ATTR_CLEAR_BUFFER		= "Clear Buffers On Bind";


	public:
		AttrViewer_Mesh() {}
		virtual ~AttrViewer_Mesh() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		Mesh*	_target;
	};
}