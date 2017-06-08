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
		cchar* const TITLE					= "Mesh";

		cchar* const ATTR_NAME				= "Name";
		cchar* const ATTR_MESH_TYPE			= "Mesh Type";
		cchar* const MESH_TYPE_LINE			= "LINE";
		cchar* const MESH_TYPE_POLYGON		= "POLYGON";
		cchar* const ATTR_LOAD_TYPE			= "Load Type";
		cchar* const LOAD_TYPE_EXTERNAL		= "EXTERNAL";
		cchar* const LOAD_TYPE_SCRIPTED		= "SCRIPTED";
		cchar* const ATTR_FILE_PATH			= "File Path";
		cchar* const ATTR_CLEAR_BUFFER		= "Clear Buffers On Bind";


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