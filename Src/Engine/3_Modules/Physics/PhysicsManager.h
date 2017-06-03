// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"

namespace CYRED
{
	class GameObject;
	class RigidBody;
}


namespace CYRED
{
	ABSTRACT class DLL PhysicsManager
	{
		DECLARE_LOCAL_SINGLETON( PhysicsManager )


	public:
		virtual void Initialize			()							PURE_VIRTUAL;
		virtual void Finalize			()							PURE_VIRTUAL;
		virtual void Update				()							PURE_VIRTUAL;

		virtual void RegisterObject		( GameObject* gameObject )	PURE_VIRTUAL;
		virtual void UnregisterObject	( GameObject* gameObject )	PURE_VIRTUAL;
		virtual void UpdateRigidBody	( RigidBody* rigidBody )	PURE_VIRTUAL;

	};
}