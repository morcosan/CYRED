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
		const char* TITLE					= "Morph";

		const char* ATTR_NAME				= "Name";
		const char* ATTR_FILE_PATHS			= "File Paths";


	public:
		AttrViewer_Morph() {}
		virtual ~AttrViewer_Morph() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	private:
		Morph*	_target;
	};
}