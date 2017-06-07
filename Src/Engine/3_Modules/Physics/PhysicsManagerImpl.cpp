// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "PhysicsManagerImpl.h"

#include "Components\RigidBody.h"
#include "../Time/TimeManager.h"
#include "../../2_BuildingBlocks/Components/Transform.h"
#include "../../2_BuildingBlocks/GameObject.h"
#include "../Script/Components/Scripter.h"

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
using namespace NonAPI;


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
	Iterator<const GameObject*, _RigidBodyData> iter = _rigidBodies.GetIterator();
	while ( iter.HasNext() ) {
		// clear memory
		PTR_FREE( iter.GetValue().physicsBody->getMotionState() );
		PTR_FREE( iter.GetValue().physicsBody->getCollisionShape() );
		PTR_FREE( iter.GetValue().physicsBody );

		// next
		iter.Next();
	}

	// clear lists
	_rigidBodies.Clear();
	_scripters.Clear();
}


void PhysicsManagerImpl::Update()
{
	// update physics from gameobject
	Iterator<const GameObject*, _RigidBodyData> iter = _rigidBodies.GetIterator();
	while ( iter.HasNext() ) {
		const GameObject* gameObject = iter.GetKey();
		_RigidBodyData& data		 = iter.GetValue();

		if ( gameObject->IsEnabled() && gameObject->IsInScene()
			 && data.rigidBody->IsEnabled() 
			 && data.transform->IsEnabled() ) 
		{
			// add to world
			if ( !data.isAddedToWorld ) {
				_dynamicsWorld->addRigidBody( data.physicsBody );
				data.isAddedToWorld = TRUE;
			}
			// get physics transform
			btTransform bodyTran = data.physicsBody->getWorldTransform();
			// update position
			Vector3 pos = data.transform->GetPositionWorld();
			bodyTran.setOrigin( btVector3( pos.x, pos.y, pos.z ) );
			// update rotation
			Quaternion rot = data.transform->GetRotationWorld();
			bodyTran.setRotation( btQuaternion( rot.x, rot.y, rot.z, rot.w ) );
			// update transform
			data.physicsBody->setWorldTransform( bodyTran );
			// update scale
			Vector3 scale = data.transform->GetScaleWorld();
			data.physicsBody->getCollisionShape()->setLocalScaling( btVector3( scale.x, scale.y, scale.z ) );
		}
		else {
			// remove from world
			if ( data.isAddedToWorld ) {
				_dynamicsWorld->removeRigidBody( data.physicsBody );
				data.isAddedToWorld = FALSE;
			}
		}

		// next
		iter.Next();
	}


	// update physics
	_dynamicsWorld->stepSimulation( TimeManager::Singleton()->GetDeltaTime() );

	// check collisions
	_CheckCollisions();


	// update gameobject from physics
	iter = _rigidBodies.GetIterator();
	while ( iter.HasNext() ) {
		const GameObject* gameObject = iter.GetKey();
		_RigidBodyData& data		 = iter.GetValue();

		if ( gameObject->IsEnabled() && data.rigidBody->IsEnabled() 
			 && data.transform->IsEnabled() ) 
		{
			// get physics transform
			Transform* transform = data.rigidBody->GetGameObject()->GetComponent<Transform>();
			btTransform bodyTran = data.physicsBody->getWorldTransform();
			// update position
			btVector3 pos = bodyTran.getOrigin();
			transform->SetPositionWorld( Vector3( pos.getX(), pos.getY(), pos.getZ() ) );
			// update rotation
			btQuaternion rot = bodyTran.getRotation();
			transform->SetRotationWorld( Quaternion( rot.getX(), rot.getY(), rot.getZ(), rot.getW() ) );
		}

		// next
		iter.Next();
	}
}

Scripter* PhysicsManagerImpl::RaycastFirstTarget( const Ray& ray )
{
	// do raycasting
	btVector3 start = btVector3( ray.start.x, ray.start.y, ray.start.z );
	btVector3 end	= btVector3( ray.end.x, ray.end.y, ray.end.z );
	btCollisionWorld::ClosestRayResultCallback rayCallback( start, end );
	_dynamicsWorld->rayTest( start, end, rayCallback );

	// test result
	if ( rayCallback.hasHit() ) {
		// get physics body
		const btRigidBody* physicsBody = CAST_S( const btRigidBody*, rayCallback.m_collisionObject );
		// get scripter
		_ScripterData data = _scripters.Get( physicsBody );
		// return scripter
		return data.scripter;
	}

	// no hit
	return NULL;
}


void PhysicsManagerImpl::RegisterObject( GameObject* gameObject )
{
	if ( !_rigidBodies.Has( gameObject ) ) {
		_CreateRigidBodyData( gameObject );
	}
}


void PhysicsManagerImpl::UnregisterObject( GameObject* gameObject )
{
	if ( _rigidBodies.Has( gameObject ) ) {
		_RigidBodyData& data = _rigidBodies.Get( gameObject );

		// remove from world
		if ( data.isAddedToWorld ) {
			_dynamicsWorld->removeRigidBody( data.physicsBody );
		}

		// remove from scripter list
		_scripters.Erase( data.physicsBody );

		// remove from rigid body list
		PTR_FREE( data.physicsBody );
		_rigidBodies.Erase( gameObject );
	}
}


void PhysicsManagerImpl::UpdateRigidBody( RigidBody* rigidBody )
{
	GameObject* gameObject = rigidBody->GetGameObject();
	if ( _rigidBodies.Has( gameObject ) ) {
		_RigidBodyData& data = _rigidBodies.Get( gameObject );

		// remove from world
		if ( data.isAddedToWorld ) {
			_dynamicsWorld->removeRigidBody( data.physicsBody );
		}

		// remove from scripter list
		_scripters.Erase( data.physicsBody );

		// clear memory
		PTR_FREE( data.physicsBody );

		// recreate data
		_CreateRigidBodyData( gameObject );
	}
}


void PhysicsManagerImpl::RegisterScripter( Scripter* scripter )
{
	// check if rigid body created
	GameObject* gameObject = scripter->GetGameObject();
	if ( _rigidBodies.Has( gameObject ) ) {
		// get rigid body data
		_RigidBodyData& rbData = _rigidBodies.Get( gameObject );
		// update scripter
		_ScripterData& sData = _scripters.Get( rbData.physicsBody );
		sData.scripter = scripter;
	}
}


void PhysicsManagerImpl::UnregisterScripter( Scripter* scripter )
{
	// check if rigid body created
	GameObject* gameObject = scripter->GetGameObject();
	if ( _rigidBodies.Has( gameObject ) ) {
		// get rigid body data
		_RigidBodyData& rbData = _rigidBodies.Get( gameObject );
		// update scripter
		_ScripterData& sData = _scripters.Get( rbData.physicsBody );
		sData.scripter = NULL;
	}
}


void PhysicsManagerImpl::_CreateRigidBodyData( GameObject* gameObject )
{
	// get rigit body
	RigidBody* rigidBody = gameObject->GetComponent<RigidBody>();
	Transform* transform = gameObject->GetComponent<Transform>();
	Scripter* scripter = gameObject->GetComponent<Scripter>();

	if ( gameObject->IsInScene() && rigidBody != NULL && transform != NULL ) {
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
		// create physicsBody
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			rigidBody->GetMass(),
			motionstate,
			collisionShape,
			inertia
		);
		btRigidBody* physicsBody = new btRigidBody( rigidBodyCI );
		// set trigger
		if ( rigidBody->IsTrigger() ) {
			physicsBody->setCollisionFlags( physicsBody->getCollisionFlags() |
											btCollisionObject::CF_NO_CONTACT_RESPONSE );
		}

		// add to list
		_rigidBodies.Set( gameObject, _RigidBodyData {
			physicsBody,
			transform,
			rigidBody,
			FALSE
		} );

		// add to scripter list
		_scripters.Set( physicsBody, _ScripterData {
			gameObject,
			scripter,
			rigidBody
		} );
	}
}


void PhysicsManagerImpl::_CheckCollisions()
{
	int numManifolds = _dispatcher->getNumManifolds();
	for ( int i = 0; i < numManifolds; i++ ) {
		btPersistentManifold* contactManifold = _dispatcher->getManifoldByIndexInternal( i );
		const btCollisionObject* ob0 = contactManifold->getBody0();
		const btCollisionObject* ob1 = contactManifold->getBody1();

		// get rigid bodies
		const btRigidBody* physicsBody0 = CAST_S( const btRigidBody*, ob0 );
		const btRigidBody* physicsBody1 = CAST_S( const btRigidBody*, ob1 );
		// get scripters
		_ScripterData& sData0 = _scripters.Get( physicsBody0 );
		_ScripterData& sData1 = _scripters.Get( physicsBody1 );
		// check for trigger
		bool isTrigger = sData0.rigidBody->IsTrigger() || sData1.rigidBody->IsTrigger();
		// call OnCollision
		if ( sData0.scripter != NULL ) {
			sData0.scripter->OnCollision( sData1.gameObject, isTrigger );
		}
		if ( sData1.scripter != NULL ) {
			sData1.scripter->OnCollision( sData0.gameObject, isTrigger );
		}
	}
}
