// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"
#include "../../2_BuildingBlocks/Math/Vector3.h"

namespace CYRED
{
	class GameObject;
	class RigidBody;
	class Scripter;

	struct Ray
	{
		Vector3 start;
		Vector3 end;
	};
}


namespace CYRED
{
	ABSTRACT class DLL PhysicsManager
	{
		DECLARE_LOCAL_SINGLETON( PhysicsManager )


	public:
		virtual void		Initialize			()					PURE_VIRTUAL;
		virtual void		Finalize			()					PURE_VIRTUAL;
		virtual void		Update				()					PURE_VIRTUAL;

		virtual Scripter*	RaycastFirstTarget	( const Ray& ray )	PURE_VIRTUAL;
	};
}