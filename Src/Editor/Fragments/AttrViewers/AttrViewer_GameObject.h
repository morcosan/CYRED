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
		AttrViewer_GameObject();
		virtual ~AttrViewer_GameObject() {}


	public:
		void OnSelect_Target	( void* target )	override;


	protected:
		void _OnUpdate_GUI		()	override;
		void _OnUpdate_Target	()	override; 


	protected:
		GameObject* _target;
		UInt		_ignoreCounter;
	};
}