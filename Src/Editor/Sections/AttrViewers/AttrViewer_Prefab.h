// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
		cchar* const TITLE				= "Prefab";

		cchar* const ATTR_NAME			= "Name";
		cchar* const GROUP_GAMEOBJECTS	= "GameObjects";


	public:
		AttrViewer_Prefab() {}
		virtual ~AttrViewer_Prefab() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _UpdateGUI		()					override;
		void _UpdateTarget	()					override;


	private:
		Prefab*	_target;
	};
}