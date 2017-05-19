// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/Component.h"
#include "../../../2_BuildingBlocks/Data/DataArray.h"
#include "../../../3_Modules/Event/Sections/IEventListener.h"


namespace CYRED
{
	class Asset;
	class Script;
}


namespace CYRED
{
	class DLL Scripter : public Component, public IEventListener
	{
	public:
		Scripter( GameObject* gameObject = NULL );
		virtual ~Scripter();


	public:
		void OnHierarchyChange	()	override {}

		void OnEvent( EventType eType, void* eData ) override;


	public:
		void Clone( Component* clone ) const override;


	public:
		Script* GetScript		( int index )	const;
		int	GetScriptsCount	()				const;

		void	SetScript		( int index, const char* scriptUID );
		void	ClearScripts	();


	protected:
		void _OnEnable	() override {}
		void _OnStart	( bool isRuntime ) override;
		void _OnUpdate	( bool isRuntime ) override;


	protected:
		DataArray<Script*>	_scripts;
		DataArray<Asset*>	_scriptsAsset;
	};
}