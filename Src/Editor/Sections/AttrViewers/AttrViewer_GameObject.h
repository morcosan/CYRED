// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"


namespace CYRED
{
	class GameObject;
}


namespace CYRED
{
	class AttrViewer_GameObject : public AttrViewer
	{
	public:
		cchar* TITLE		= "GameObject";
		cchar* ATTR_NAME	= "Name";
		cchar* ATTR_UID	= "UID";


	public:
		AttrViewer_GameObject() {}
		virtual ~AttrViewer_GameObject() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _UpdateGUI		()					override;
		void _UpdateTarget	()					override;


	private:
		GameObject* _target;
	};
}