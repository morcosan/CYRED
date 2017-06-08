// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"

namespace CYRED
{
	class Font;
}


namespace CYRED
{
	class AttrViewer_Font : public AttrViewer
	{
	public:
		cchar* const TITLE			= "Font";

		cchar* const ATTR_NAME		= "Name";
		cchar* const ATTR_FILE_PATH	= "File Path";


	public:
		AttrViewer_Font() {}
		virtual ~AttrViewer_Font() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _UpdateGUI			()					override;
		void _UpdateTarget		()					override;


	private:
		Font*	_target;
	};
}