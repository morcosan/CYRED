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

		const Char* ATTR_NAME				= "Name";
		const Char* ATTR_SHADER				= "Shader";
		const Char* ATTR_WIREFRAME			= "Wireframe";
		const Char* ATTR_LINE_WIDTH			= "Line Width";
		const Char* ATTR_CULL_FACE			= "Cull Face";
		const Char* ATTR_PROPERTIES			= "Properties";
		const Char* ATTR_PROP_TYPE_INT		= "Integers";
		const Char* ATTR_PROP_TYPE_FLOAT	= "Floats";
		const Char* ATTR_PROP_TYPE_VECTOR2	= "Vector2s";
		const Char* ATTR_PROP_TYPE_VECTOR3	= "Vector3s";
		const Char* ATTR_PROP_TYPE_VECTOR4	= "Vector4s";
		const Char* ATTR_PROP_TYPE_TEXTURE	= "Textures";
		const Char* ATTR_PROP_NAME			= "Name";
		const Char* ATTR_PROP_VALUE			= "Value";


	public:
		AttrViewer_Material() {}
		virtual ~AttrViewer_Material() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		Material*	_target;
	};
}