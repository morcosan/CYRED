// Copyright (c) 2015 Morco (www.morco.ro)
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
		const Char* TITLE					= "Morph";

		const Char* ATTR_NAME				= "Name";
		const Char* ATTR_FILE_PATHS			= "File Paths";


	public:
		AttrViewer_Morph() {}
		virtual ~AttrViewer_Morph() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		Morph*	_target;
	};
}