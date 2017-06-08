// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"


namespace CYRED
{
	class Scene;
}


namespace CYRED
{
	class AttrViewer_Scene : public AttrViewer
	{
	public:
		cchar* const TITLE		= "Scene";

		cchar* const ATTR_NAME	= "Name";


	public:
		AttrViewer_Scene() {}
		virtual ~AttrViewer_Scene() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _UpdateGUI		()					override;
		void _UpdateTarget	()					override;


	private:
		Scene* _target;
	};
}