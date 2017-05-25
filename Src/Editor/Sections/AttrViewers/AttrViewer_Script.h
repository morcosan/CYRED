// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
		cchar* TITLE					= "Script";

		cchar* ATTR_NAME				= "Name";
		cchar* ATTR_RUN_EDITOR			= "Run in Editor";
		cchar* ATTR_FILE_PATHS			= "File Paths";
		cchar* GROUP_VARIABLES			= "Variables";
		cchar* GROUP_FUNCTIONS			= "Functions";


	public:
		AttrViewer_Script() {}
		virtual ~AttrViewer_Script() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _UpdateGUI		()					override;
		void _UpdateTarget	()					override;


	private:
		Script*	_target;
	};
}