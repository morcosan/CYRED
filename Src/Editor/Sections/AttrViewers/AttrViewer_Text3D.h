// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"

namespace CYRED
{
	class Text3D;
}


namespace CYRED
{
	class AttrViewer_Text3D : public AttrViewer
	{
	public:
		cchar* TITLE			= "Text3D";

		cchar* ATTR_TEXT		= "Text";
		cchar* ATTR_TEXT_COLOR	= "Color";
		cchar* ATTR_V_ALIGN		= "V. Align";
		cchar* ATTR_H_ALIGN		= "H. Align";
		cchar* ATTR_FONT		= "Font";
		cchar* ATTR_FONT_SIZE	= "Size";
		cchar* ATTR_SHADER		= "Shader";
		cchar* V_ALIN_TOP		= "Top";
		cchar* V_ALIN_MIDDLE	= "Middle";
		cchar* V_ALIN_BOTTOM	= "Bottom";
		cchar* H_ALIN_LEFT		= "Left";
		cchar* H_ALIN_MIDDLE	= "Middle";
		cchar* H_ALIN_RIGHT		= "Right";


	public:
		AttrViewer_Text3D() {}
		virtual ~AttrViewer_Text3D() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _UpdateGUI			()					override;
		void _UpdateTarget		()					override;


	private:
		Text3D*	_target;
	};
}