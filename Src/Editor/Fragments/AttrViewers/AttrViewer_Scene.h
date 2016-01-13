// Copyright (c) 2015 Morco (www.morco.ro)
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
		const Char* TITLE				= "Scene";

		const Char* ATTR_NAME			= "Name";


	public:
		AttrViewer_Scene() {}
		virtual ~AttrViewer_Scene() {}


	public:
		void OnSelect_Target	( void* target )	override;
	

	protected:
		void _OnUpdate_GUI		()	override;
		void _OnUpdate_Target	()	override;


	protected:
		Scene* _target;
	};
}