// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "PhysicsManagerImpl.h"

#include "Components\RigidBody.h"
#include "../Time/TimeManager.h"
#include "../../2_BuildingBlocks/Components/Transform.h"
#include "../../2_BuildingBlocks/GameObject.h"

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
	_broadphase	= new btDbvtBroadphase();
	_collConfig	= new btDefaultCollisionConfiguration();
	_dispatcher	= new btCollisionDispatcher( _collConfig );
	_solver		= new btSequentialImpulseConstraintSolver();
	// create physics world
	_dynamicsWorld = new btDiscreteDynamicsWorld( 
		_dispatcher, _broadphase, _solver, _collConfig 
	);
	_dynamicsWorld->setGravity( btVector3( 0, -9.81f, 0 ) );
}


void PhysicsManagerImpl::Finalize()
{
	if ( !_isInitialized ) {
		return;
	}

	// destroy world first
	PTR_FREE( _dynamicsWorld );
	PTR_FREE( _collConfig );
	PTR_FREE( _broadphase );
	PTR_FREE( _dispatcher );
	PTR_FREE( _solver );

	// delete rigid bodies
	Iterator<RigidBody*, btRigidBody*> iter = _rigidBodies.GetIterator();
	while ( iter.HasNext() ) {
		// clear memory
		PTR_FREE( iter.GetValue()->getMotionState() );
		PTR_FREE( iter.GetValue()->getCollisionShape() );
		PTR_FREE( iter.GetValue() );

		// next
		iter.Next();
	}
}


void PhysicsManagerImpl::Update()
{
	// update physics from gameobject
	Iterator<RigidBody*, btRigidBody*> iter = _rigidBodies.GetIterator();
	while ( iter.HasNext() ) {
		RigidBody* rigidBody = iter.GetKey();
		btRigidBody* body = iter.GetValue();

		// get transform
		Transform* transform = rigidBody->GetGameObject()->GetComponent<Transform>();
		btTransform bodyTran = body->getWorldTransform();
		// update position
		Vector3 pos = transform->GetPositionWorld();
		bodyTran.setOrigin( btVector3( pos.x, pos.y, pos.z ) );
		// update rotation
		Quaternion rot = transform->GetRotationWorld();
		bodyTran.setRotation( btQuaternion( rot.x, rot.y, rot.z, rot.w ) );
		// update transform
		body->setWorldTransform( bodyTran );
		// update scale
		Vector3 scale = transform->GetScaleWorld();
		body->getCollisionShape()->setLocalScaling( btVector3( scale.x, scale.y, scale.z ) );

		// next
		iter.Next();
	}

	_dynamicsWorld->stepSimulation( TimeManager::Singleton()->GetDeltaTime() );

	// update gameobject from physics
	iter = _rigidBodies.GetIterator();
	while ( iter.HasNext() ) {
		RigidBody* rigidBody = iter.GetKey();
		btRigidBody* body = iter.GetValue();

		// get transform
		Transform* transform = rigidBody->GetGameObject()->GetComponent<Transform>();
		btTransform bodyTran = body->getWorldTransform();
		// update position
		btVector3 pos = bodyTran.getOrigin();
		transform->SetPositionWorld( Vector3( pos.getX(), pos.getY(), pos.getZ() ) );
		// update rotation
		btQuaternion rot = bodyTran.getRotation();
		transform->SetRotationWorld( Quaternion( rot.getX(), rot.getY(), rot.getZ(), rot.getW() ) );

		// next
		iter.Next();
	}
}


void PhysicsManagerImpl::RegisterRigidBody( RigidBody* rigidBody )
{
	if ( !_rigidBodies.Has( rigidBody ) ) {
		// create shape
		btCollisionShape* collisionShape = NULL;
		Vector3 shapeSize = rigidBody->GetShapeSize();
		switch ( rigidBody->GetShapeType() ) {
			case CollisionShapeType::BOX:
				collisionShape = new btBoxShape( btVector3( shapeSize.x, shapeSize.y, shapeSize.x ) );
				break;

			case CollisionShapeType::SPHERE:
				collisionShape = new btSphereShape( shapeSize.x );
				break;
		}
		// create motion state
		btMotionState* motionstate = new btDefaultMotionState();
		// calculate inertia
		btVector3 inertia;
		collisionShape->calculateLocalInertia( rigidBody->GetMass(), inertia );
		// create body
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			rigidBody->GetMass(),
			motionstate,
			collisionShape,
			inertia
		);
		btRigidBody* body = new btRigidBody( rigidBodyCI );

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
		PTR_FREE( body );
	}
}


void PhysicsManagerImpl::UpdateRigidBody( RigidBody* rigidBody )
{
	if ( _rigidBodies.Has( rigidBody ) ) {
		btRigidBody* body = _rigidBodies.Get( rigidBody );

		// remove from world
		_dynamicsWorld->removeRigidBody( body );
		// delete
		PTR_FREE( body );

		// create shape
		btCollisionShape* collisionShape = NULL;
		Vector3 shapeSize = rigidBody->GetShapeSize();
		switch ( rigidBody->GetShapeType() ) {
			case CollisionShapeType::BOX:
				collisionShape = new btBoxShape( btVector3( shapeSize.x, shapeSize.y, shapeSize.x ) );
				break;

			case CollisionShapeType::SPHERE:
				collisionShape = new btSphereShape( shapeSize.x );
				break;
		}
		// create motion state
		btMotionState* motionstate = new btDefaultMotionState();
		// calculate inertia
		btVector3 inertia;
		collisionShape->calculateLocalInertia( rigidBody->GetMass(), inertia );
		// create new body
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			rigidBody->GetMass(),
			motionstate,
			collisionShape,
			inertia
		);
		body = new btRigidBody( rigidBodyCI );

		// add to world
		_dynamicsWorld->addRigidBody( body );

		// add to list
		_rigidBodies.Set( rigidBody, body );
	}
}
