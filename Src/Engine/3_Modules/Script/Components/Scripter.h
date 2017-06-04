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
		void OnHierarchyChange	()									override {}
		void OnAdded			()									override;
		void OnRemoved			()									override;
		void OnEvent			( int eventType, void* eventData )	override;
		void Clone				( Component* clone ) const			override;


	public:
		void	OnCollision		( GameObject* other );

		Script* GetScript		( int index )	const;
		int		GetScriptsCount	()				const;

		void	SetScript		( int index, cchar* scriptUID );
		void	ClearScripts	();


	private:
		void _OnEnable	() override {}
		void _OnStart	( bool isRuntime ) override;
		void _OnUpdate	( bool isRuntime ) override;


	private:
		DataArray<Script*>	_scripts;
		DataArray<Asset*>	_scriptsAsset;
	};
}