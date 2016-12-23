// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"


namespace CYRED
{
	class Script;
}


namespace CYRED
{
	class AttrViewer_Script : public AttrViewer
	{
	public:
		const Char* TITLE					= "Script";

		const Char* ATTR_NAME				= "Name";
		const Char* ATTR_RUN_EDITOR			= "Run in Editor";
		const Char* ATTR_FILE_PATHS			= "File Paths";


	public:
		AttrViewer_Script() {}
		virtual ~AttrViewer_Script() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		Script*	_target;
	};
}