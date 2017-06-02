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
	namespace NotAPI
	{
		class PhysicsManagerImpl : public PhysicsManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( PhysicsManagerImpl )


		public:
			void Initialize			()							override;
			void Finalize			()							override;
			void Update				()							override;

			void RegisterRigidBody	( RigidBody* rigidBody )	override;
			void UnregisterRigidBody( RigidBody* rigidBody )	override;
			void UpdateRigidBody	( RigidBody* rigidBody )	override;


		private:
			btDynamicsWorld*			_dynamicsWorld;
			btBroadphaseInterface*		_broadphase;
			btCollisionConfiguration*	_collConfig;
			btCollisionDispatcher*		_dispatcher;
			btConstraintSolver*			_solver;

			DataMap<RigidBody*, btRigidBody*>	_rigidBodies;
		};
	}
}