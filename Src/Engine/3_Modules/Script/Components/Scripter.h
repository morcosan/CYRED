// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/Component.h"
#include "../../../2_BuildingBlocks/Data/DataArray.h"


namespace CYRED
{
	class Script;
}


namespace CYRED
{
	namespace COMP
	{
		class DLL Scripter : public Component
		{
		public:
			Scripter( GameObject* gameObject = NULL );
			virtual ~Scripter() {}


		public:
			void OnHierarchyChange	()	override {}


		public:
			Script* GetScript		( UInt index )	const;
			UInt	GetScriptsCount	()				const;

			void	SetScript		( UInt index, const Char* scriptUID );
			void	ClearScripts	();


		protected:
			void _OnEnable	() override {}
			void _OnStart	( Bool isRuntime ) override;
			void _OnUpdate	( Bool isRuntime ) override;


		protected:
			DataArray<Script*>	_scripts;
		};
	}
}