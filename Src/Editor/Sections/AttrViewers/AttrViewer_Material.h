// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
		const char* TITLE				= "Material";

		const char* ATTR_NAME				= "Name";
		const char* ATTR_SHADER				= "Shader";
		const char* ATTR_WIREFRAME			= "Wireframe";
		const char* ATTR_LINE_WIDTH			= "Line Width";
		const char* ATTR_CULL_FACE			= "Cull Face";
		const char* ATTR_PROPERTIES			= "Properties";
		const char* ATTR_PROP_TYPE_INT		= "Integers";
		const char* ATTR_PROP_TYPE_FLOAT	= "Floats";
		const char* ATTR_PROP_TYPE_VECTOR2	= "Vector2s";
		const char* ATTR_PROP_TYPE_VECTOR3	= "Vector3s";
		const char* ATTR_PROP_TYPE_VECTOR4	= "Vector4s";
		const char* ATTR_PROP_TYPE_TEXTURE	= "Textures";
		const char* ATTR_PROP_NAME			= "Name";
		const char* ATTR_PROP_VALUE			= "Value";


	public:
		AttrViewer_Material() {}
		virtual ~AttrViewer_Material() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	private:
		Material*	_target;

	private:
		void _ChangeVisibility	();
	};
}