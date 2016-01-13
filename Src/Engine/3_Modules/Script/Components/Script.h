// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/Component.h"


namespace CYRED
{
	namespace COMP
	{
		ABSTRACT class DLL Script : public Component
		{
		public:
			Script( GameObject* gameObject = NULL );
			virtual ~Script() {}


		public:
			void OnHierarchyChange()	override {}


		public:
			void Start	();
			void Update	();

			Bool DoesRunInEditor();
			void SetRunInEditor( Bool value );
			Bool DoesNeedStart();
			

		protected:
			void _OnEnable() override {}


		protected:
			Bool	_runInEditor;
			Bool	_needStart;

			virtual void _OnStart	() PURE_VIRTUAL;
			virtual void _OnUpdate() PURE_VIRTUAL;
		};
	}
}