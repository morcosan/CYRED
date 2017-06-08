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
		cchar* const TITLE			= "Scripter";

		cchar* const ATTR_SCRIPTS	= "Scripts";


	public:
		AttrViewer_Scripter() {}
		virtual ~AttrViewer_Scripter() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _UpdateGUI		()					override;
		void _UpdateTarget	()					override;


	private:
		Scripter*	_target;
	};
}