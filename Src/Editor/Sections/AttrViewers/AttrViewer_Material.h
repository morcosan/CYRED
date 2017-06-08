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
		cchar* const TITLE					= "Material";

		cchar* const ATTR_NAME				= "Name";
		cchar* const ATTR_SHADER			= "Shader";
		cchar* const ATTR_WIREFRAME			= "Wireframe";
		cchar* const ATTR_LINE_WIDTH		= "Line Width";
		cchar* const ATTR_CULL_FACE			= "Cull Face";
		cchar* const ATTR_PROPERTIES		= "Properties";
		cchar* const ATTR_PROP_TYPE_INT		= "Integers";
		cchar* const ATTR_PROP_TYPE_FLOAT	= "Floats";
		cchar* const ATTR_PROP_TYPE_VECTOR2	= "Vector2s";
		cchar* const ATTR_PROP_TYPE_VECTOR3	= "Vector3s";
		cchar* const ATTR_PROP_TYPE_VECTOR4	= "Vector4s";
		cchar* const ATTR_PROP_TYPE_TEXTURE	= "Textures";
		cchar* const ATTR_PROP_NAME			= "Name";
		cchar* const ATTR_PROP_VALUE		= "Value";
		cchar* const ATTR_PICKING_PROXY		= "Picking Proxy";


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