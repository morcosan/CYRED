// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"


namespace CYRED
{
	class Shader;
}


namespace CYRED
{
	class AttrViewer_Shader : public AttrViewer
	{
	public:
		cchar* const TITLE			= "Shader";

		cchar* const ATTR_NAME		= "Name";
		cchar* const ATTR_VERTEX	= "Vertex FilePath";
		cchar* const ATTR_GEOMETRY	= "Geometry FilePath";
		cchar* const ATTR_FRAGMENT	= "Fragment FilePath";


	public:
		AttrViewer_Shader() {}
		virtual ~AttrViewer_Shader() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _UpdateGUI		()					override;
		void _UpdateTarget	()					override;


	private:
		Shader* _target;
	};
}