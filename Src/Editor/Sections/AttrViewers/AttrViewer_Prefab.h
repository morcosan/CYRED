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
		const char* TITLE				= "Prefab";

		const char* ATTR_NAME			= "Name";
		const char* GROUP_GAMEOBJECTS	= "GameObjects";


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