// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/Component.h"


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
			void OnHierarchyChange()	override {}


		public:
			void	OnStart	();
			void	OnUpdate();

			Bool	NeedStart() const;
			Script* GetScript() const;

			void	SetScript( Script* script );
			

		protected:
			void _OnEnable() override {}


		protected:
			Bool	_needStart;
			Script*	_script;

			virtual void _OnStart	() {}
			virtual void _OnUpdate	() {}
		};
	}
}