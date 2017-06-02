// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "PhysicsManagerImpl.h"

#include "Components\RigidBody.h"
#include "../Time/TimeManager.h"

#include "BulletDynamics\Dynamics\btDynamicsWorld.h"
#include "BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h"
#include "BulletDynamics\Dynamics\btRigidBody.h"
#include "BulletDynamics\ConstraintSolver\btConstraintSolver.h"
#include "BulletDynamics\ConstraintSolver\btSequentialImpulseConstraintSolver.h"

#include "BulletCollision\BroadphaseCollision\btBroadphaseInterface.h"
#include "BulletCollision\BroadphaseCollision\btDbvtBroadphase.h"
#include "BulletCollision\CollisionDispatch\btDefaultCollisionConfiguration.h"
#include "BulletCollision\CollisionDispatch\btCollisionDispatcher.h"
#include "BulletCollision\CollisionShapes\btBoxShape.h"
#include "BulletCollision\CollisionShapes\btSphereShape.h"

#include "LinearMath\btDefaultMotionState.h"


using namespace CYRED;
using namespace NotAPI;


//! deferred definition of SceneManager
DEFINE_LOCAL_SINGLETON( PhysicsManager, PhysicsManagerImpl )
DEFINE_LOCAL_SINGLETON_IMPL( PhysicsManagerImpl )


void PhysicsManagerImpl::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = true;

	// create bullet configuration
	_broadphase	= Memory::Alloc<btDbvtBroadphase>();
	_collConfig	= Memory::Alloc<btDefaultCollisionConfiguration>();
	_dispatcher	= Memory::Alloc<btCollisionDispatcher>( _collConfig );
	_solver		= Memory::Alloc<btSequentialImpulseConstraintSolver>();
	// create physics world
	_dynamicsWorld = Memory::Alloc<btDiscreteDynamicsWorld>( 
		_dispatcher, _broadphase, _solver, _collConfig 
	);
	_dynamicsWorld->setGravity( btVector3( 0, -9.81f, 0 ) );
}


void PhysicsManagerImpl::Finalize()
{
	if ( !_isInitialized ) {
		return;
	}

	// delete rigid bodies
	Iterator<RigidBody*, btRigidBody*> iter = _rigidBodies.GetIterator();
	while ( iter.HasNext() ) {
		// clear memory
		Memory::Free( iter.GetValue()->getMotionState() );
		Memory::Free( iter.GetValue()->getCollisionShape() );
		Memory::Free( iter.GetValue() );
		// next
		iter.Next();
	}

	// clear memory
	Memory::Free( _collConfig );
	Memory::Free( _broadphase );
	Memory::Free( _dispatcher );
	Memory::Free( _solver );

	// bullet alignment bug
	//Memory::Free( _dynamicsWorld );
}


void PhysicsManagerImpl::Update()
{
	_dynamicsWorld->stepSimulation( TimeManager::Singleton()->GetDeltaTime() );
}


void PhysicsManagerImpl::RegisterRigidBody( RigidBody* rigidBody )
{
	if ( !_rigidBodies.Has( rigidBody ) ) {
		// create shape
		btCollisionShape* collisionShape = NULL;
		Vector3 shapeSize = rigidBody->GetShapeSize();
		switch ( rigidBody->GetShapeType() ) {
			case CollisionShapeType::BOX:
				collisionShape = Memory::Alloc<btBoxShape>( btVector3( shapeSize.x, shapeSize.y, shapeSize.x ) );
				break;

			case CollisionShapeType::SPHERE:
				collisionShape = Memory::Alloc<btSphereShape>( shapeSize.x );
				break;
		}
		// create bullet rigid body
		btMotionState* motionstate = new btDefaultMotionState();
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			rigidBody->GetMass(), motionstate, collisionShape, btVector3( 0, 0, 0 )    
		);
		btRigidBody* body = Memory::Alloc<btRigidBody>( rigidBodyCI );

		// add to world
		_dynamicsWorld->addRigidBody( body );
		// add to list
		_rigidBodies.Set( rigidBody, body );
	}
}


void PhysicsManagerImpl::UnregisterRigidBody( RigidBody* rigidBody )
{
	if ( _rigidBodies.Has( rigidBody ) ) {
		btRigidBody* body = _rigidBodies.Get( rigidBody );

		// remove from world
		_dynamicsWorld->removeRigidBody( body );
		// remove from list
		_rigidBodies.Erase( rigidBody );

		// clear memory
		// bullet bug
		//Memory::Free( body );
	}
}
