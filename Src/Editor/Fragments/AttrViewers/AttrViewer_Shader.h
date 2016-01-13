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
		const Char* TITLE				= "Shader";

		const Char* ATTR_NAME			= "Name";
		const Char* ATTR_RENDERER		= "Renderer";
		const Char* RENDERER_FORWARD	= "FORWARD";
		const Char* ATTR_VERTEX			= "Vertex FilePath";
		const Char* ATTR_GEOMETRY		= "Geometry FilePath";
		const Char* ATTR_FRAGMENT		= "Fragment FilePath";


	public:
		AttrViewer_Shader() {}
		virtual ~AttrViewer_Shader() {}


	public:
		void OnSelect_Target	( void* target )	override;
	

	protected:
		void _OnUpdate_GUI		()	override;
		void _OnUpdate_Target	()	override;


	protected:
		Shader* _target;
	};
}