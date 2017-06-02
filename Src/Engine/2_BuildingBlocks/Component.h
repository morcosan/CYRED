// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
			, RIGID_BODY
		};
	}
	typedef	Enum_ComponentType::Enum	ComponentType;
}


namespace CYRED
{
	ABSTRACT class DLL Component
	{
	public:
		Component( GameObject* gameObject );
		virtual ~Component() {};


	public:
		virtual void			OnHierarchyChange	() {};
		virtual ComponentType	GetComponentType	() const;
		virtual void			OnAdded				() {};
		virtual void			OnRemoved			() {};

		virtual void			Clone				( Component* clone ) const PURE_VIRTUAL;


	public:
		void		OnUpdate		( bool isRuntime );
		

		GameObject*	GetGameObject	()				const;
		bool		IsEnabled		()				const;
		bool		DoesEmitEvents	()				const;

		void		SetEnabled		( bool value );
		void		SetEmitEvents	( bool value );


	protected:
		virtual void _OnEnable	() PURE_VIRTUAL;
		virtual void _OnStart	( bool isRuntime ) {}
		virtual void _OnUpdate	( bool isRuntime ) {}


	protected:
		GameObject*		_gameObject;
		bool			_enabled;
		bool			_emitEvents;
		ComponentType	_componentType;
		bool			_isFirstUpdate;
	};
}