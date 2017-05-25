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
		cchar* TITLE					= "Material";

		cchar* ATTR_NAME				= "Name";
		cchar* ATTR_SHADER				= "Shader";
		cchar* ATTR_WIREFRAME			= "Wireframe";
		cchar* ATTR_LINE_WIDTH			= "Line Width";
		cchar* ATTR_CULL_FACE			= "Cull Face";
		cchar* ATTR_PROPERTIES			= "Properties";
		cchar* ATTR_PROP_TYPE_INT		= "Integers";
		cchar* ATTR_PROP_TYPE_FLOAT		= "Floats";
		cchar* ATTR_PROP_TYPE_VECTOR2	= "Vector2s";
		cchar* ATTR_PROP_TYPE_VECTOR3	= "Vector3s";
		cchar* ATTR_PROP_TYPE_VECTOR4	= "Vector4s";
		cchar* ATTR_PROP_TYPE_TEXTURE	= "Textures";
		cchar* ATTR_PROP_NAME			= "Name";
		cchar* ATTR_PROP_VALUE			= "Value";
		cchar* ATTR_PICKING_PROXY		= "Picking Proxy";


	public:
		AttrViewer_Material() {}
		virtual ~AttrViewer_Material() {}


	private:
		void _OnInitialize	()					override;
		void _OnChangeTarget( void* target )	override;
		void _UpdateGUI		()					override;
		void _UpdateTarget	()					override;


	private:
		Material*	_target;

	private:
		void _ChangeVisibility	();
	};
}