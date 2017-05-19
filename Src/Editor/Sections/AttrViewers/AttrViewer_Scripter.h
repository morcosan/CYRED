// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"


namespace CYRED
{
	class Scripter;
}


namespace CYRED
{
	class AttrViewer_Scripter : public AttrViewer
	{
	public:
		const char* TITLE			= "Scripter";

		const char* ATTR_NAME		= "Name";
		const char* ATTR_SCRIPTS	= "Scripts";


	public:
		AttrViewer_Scripter() {}
		virtual ~AttrViewer_Scripter() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		Scripter*	_target;
	};
}