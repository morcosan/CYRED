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
		const char* TITLE				= "Scene";

		const char* ATTR_NAME			= "Name";


	public:
		AttrViewer_Scene() {}
		virtual ~AttrViewer_Scene() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	private:
		Scene* _target;
	};
}