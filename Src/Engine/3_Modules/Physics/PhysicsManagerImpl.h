// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "PhysicsManager.h"

#include "../../2_BuildingBlocks/Data/DataMap.h"

class btDynamicsWorld;
class btBroadphaseInterface;
class btCollisionConfiguration;
class btCollisionDispatcher;
class btConstraintSolver;
class btRigidBody;

namespace CYRED
{
	class Transform;
	class Scripter;
}


namespace CYRED
{
	namespace NonAPI
	{
		class PhysicsManagerImpl : public PhysicsManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( PhysicsManagerImpl )


		public:
			void Initialize			()								override;
			void Finalize			()								override;
			void Update				()								override;

			void ApplyMouseDown		( int button, const Ray& ray )	override;
			void ApplyMouseUp		( int button, const Ray& ray )	override;


		public:
			void RegisterObject		( GameObject* gameObject );
			void UnregisterObject	( GameObject* gameObject );
			void UpdateRigidBody	( RigidBody* rigidBody );
			void RegisterScripter	( Scripter* scripter );
			void UnregisterScripter	( Scripter* scripter );


		private:
			struct _RigidBodyData
			{
				btRigidBody*	physicsBody;
				Transform*		transform;
				RigidBody*		rigidBody;
				bool			isAddedToWorld;
			};
			struct _ScripterData
			{
				GameObject*		gameObject;
				Scripter*		scripter;
				RigidBody*		rigidBody;
			};

			btDynamicsWorld*			_dynamicsWorld;
			btBroadphaseInterface*		_broadphase;
			btCollisionConfiguration*	_collConfig;
			btCollisionDispatcher*		_dispatcher;
			btConstraintSolver*			_solver;

			DataMap<const GameObject*, _RigidBodyData>	_rigidBodies;
			DataMap<const btRigidBody*, _ScripterData>	_scripters;


		private:
			void _CreateRigidBodyData	( GameObject* gameObject );
			void _CheckCollisions		();
		};
	}
}