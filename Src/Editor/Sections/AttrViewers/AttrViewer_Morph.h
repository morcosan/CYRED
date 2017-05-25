// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"


namespace CYRED
{
	class Morph;
}


namespace CYRED
{
	class AttrViewer_Morph : public AttrViewer
	{
	public:
		cchar* TITLE					= "Morph";

		cchar* ATTR_NAME				= "Name";
		cchar* ATTR_FILE_PATHS			= "File Paths";


	public:
		AttrViewer_Morph() {}
		virtual ~AttrViewer_Morph() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _UpdateGUI		()					override;
		void _UpdateTarget	()					override;


	private:
		Morph*	_target;
	};
}