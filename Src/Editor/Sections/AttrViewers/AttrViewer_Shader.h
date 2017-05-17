// Copyright (c) 2015 Morco (www.morco.ro)
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
		const char* TITLE				= "Shader";

		const char* ATTR_NAME			= "Name";
		const char* ATTR_RENDERER		= "Renderer";
		const char* RENDERER_FORWARD	= "FORWARD";
		const char* ATTR_VERTEX			= "Vertex FilePath";
		const char* ATTR_GEOMETRY		= "Geometry FilePath";
		const char* ATTR_FRAGMENT		= "Fragment FilePath";


	public:
		AttrViewer_Shader() {}
		virtual ~AttrViewer_Shader() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		Shader* _target;
	};
}