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
		const char* TITLE					= "Mesh";

		const char* ATTR_NAME				= "Name";
		const char* ATTR_MESH_TYPE			= "Mesh Type";
		const char* MESH_TYPE_LINE			= "LINE";
		const char* MESH_TYPE_POLYGON		= "POLYGON";
		const char* ATTR_LOAD_TYPE			= "Load Type";
		const char* LOAD_TYPE_EXTERNAL		= "EXTERNAL";
		const char* LOAD_TYPE_SCRIPTED		= "SCRIPTED";
		const char* ATTR_FILE_PATH			= "File Path";
		const char* ATTR_CLEAR_BUFFER		= "Clear Buffers On Bind";


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