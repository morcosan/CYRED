// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"


namespace CYRED
{
	namespace COMP
	{
		class Scripter;
	}
}


namespace CYRED
{
	class AttrViewer_Scripter : public AttrViewer
	{
	public:
		const Char* TITLE			= "Scripter";

		const Char* ATTR_NAME		= "Name";
		const Char* ATTR_SCRIPTS	= "Scripts";


	public:
		AttrViewer_Scripter() {}
		virtual ~AttrViewer_Scripter() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		COMP::Scripter*	_target;
	};
}