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
		const char* TITLE					= "Script";

		const char* ATTR_NAME				= "Name";
		const char* ATTR_RUN_EDITOR			= "Run in Editor";
		const char* ATTR_FILE_PATHS			= "File Paths";
		const char* GROUP_VARIABLES			= "Variables";
		const char* GROUP_FUNCTIONS			= "Functions";


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