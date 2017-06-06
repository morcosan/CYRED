// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"
#include "../../2_BuildingBlocks/Math/Vector3.h"

namespace CYRED
{
	class GameObject;
	class RigidBody;

	struct Ray
	{
		Vector3 origin;
		Vector3 direction;
	};
}


namespace CYRED
{
	ABSTRACT class DLL PhysicsManager
	{
		DECLARE_LOCAL_SINGLETON( PhysicsManager )


	public:
		virtual void Initialize		()								PURE_VIRTUAL;
		virtual void Finalize		()								PURE_VIRTUAL;
		virtual void Update			()								PURE_VIRTUAL;

		virtual void ApplyMouseDown	( int button, const Ray& ray )	PURE_VIRTUAL;
		virtual void ApplyMouseUp	( int button, const Ray& ray )	PURE_VIRTUAL;
	};
}