// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"


namespace CYRED
{
	class Prefab;
}


namespace CYRED
{
	class AttrViewer_Prefab : public AttrViewer
	{
	public:
		const Char* TITLE				= "Prefab";

		const Char* ATTR_NAME			= "Name";
		const Char* GROUP_GAMEOBJECTS	= "GameObjects";


	public:
		AttrViewer_Prefab() {}
		virtual ~AttrViewer_Prefab() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		Prefab*	_target;
	};
}