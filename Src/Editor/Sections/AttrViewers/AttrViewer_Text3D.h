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
		cchar* const TITLE				= "Text3D";

		cchar* const ATTR_TEXT			= "Text";
		cchar* const ATTR_TEXT_SIZE		= "Size";
		cchar* const ATTR_TEXT_COLOR	= "Color";
		cchar* const ATTR_V_ALIGN		= "V. Align";
		cchar* const ATTR_H_ALIGN		= "H. Align";
		cchar* const ATTR_FONT			= "Font";
		cchar* const ATTR_MATERIAL		= "Material";
		cchar* const V_ALIN_TOP			= "TOP";
		cchar* const V_ALIN_MIDDLE		= "MIDDLE";
		cchar* const V_ALIN_BOTTOM		= "BOTTOM";
		cchar* const H_ALIN_LEFT		= "LEFT";
		cchar* const H_ALIN_MIDDLE		= "MIDDLE";
		cchar* const H_ALIN_RIGHT		= "RIGHT";


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