// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"


namespace CYRED
{
	class Material;
}


namespace CYRED
{
	class AttrViewer_Material : public AttrViewer
	{
	public:
		const Char* TITLE				= "Material";

		const Char* ATTR_NAME			= "Name";
		const Char* ATTR_SHADER			= "Shader";
		const Char* ATTR_WIREFRAME		= "Wireframe";
		const Char* ATTR_LINE_WIDTH		= "Line Width";
		const Char* ATTR_CULL_FACE		= "Cull Face";
		const Char* ATTR_PROPERTIES		= "Properties";


	public:
		AttrViewer_Material() {}
		virtual ~AttrViewer_Material() {}


	public:
		void OnSelect_Target	( void* target )	override;
	

	protected:
		void _OnUpdate_GUI		()	override;
		void _OnUpdate_Target	()	override;


	protected:
		Material*	_target;
	};
}