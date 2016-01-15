// Copyright (c) 2015 Morco (www.morco.ro)
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
		const Char* TITLE		= "GameObject";
		const Char* ATTR_NAME	= "Name";
		const Char* ATTR_UID	= "UID";


	public:
		AttrViewer_GameObject() {}
		virtual ~AttrViewer_GameObject() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		GameObject* _target;
	};
}