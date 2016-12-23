// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../1_Required/Required.h"


namespace CYRED
{
	class GameObject;

	namespace Enum_ComponentType
	{
		enum Enum
		{
			COMPONENT
			, TRANSFORM
			, CAMERA
			, LIGHT
			, MESH_RENDERING
			, MORPH_RENDERING
			, PARTICLE_EMITTER
			, SCRIPTER
		};
	}

	typedef	Enum_ComponentType::Enum	ComponentType;
}


namespace CYRED
{
	namespace COMP
	{
		ABSTRACT class DLL Component
		{
		public:
			Component( GameObject* gameObject );
			virtual ~Component() {};


		public:
			virtual void			OnHierarchyChange	() {};
			virtual void			OnUpdate			() {};
			virtual ComponentType	GetComponentType	() const;
			virtual const Char*		GetComponentSubtype	() const;


		public:
			GameObject*	GetGameObject	()				const;
			Bool		IsEnabled		()				const;
			Bool		DoesEmitEvents	()				const;

			void		SetEnabled		( Bool value );
			void		SetEmitEvents	( Bool value );


		protected:
			virtual void _OnEnable() PURE_VIRTUAL;


		protected:
			GameObject*		_gameObject;
			Bool			_enabled;
			Bool			_emitEvents;
			ComponentType	_componentType;
			const Char*		_componentSubtype;
		};
	}
}