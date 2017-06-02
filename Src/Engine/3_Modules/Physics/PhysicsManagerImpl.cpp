// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "PhysicsManagerImpl.h"

#include "BulletDynamics\Dynamics\btDynamicsWorld.h"
#include "BulletDynamics\Dynamics\btDiscreteDynamicsWorld.h"
#include "BulletCollision\BroadphaseCollision\btBroadphaseInterface.h"
#include "BulletCollision\BroadphaseCollision\btDbvtBroadphase.h"
#include "BulletCollision\CollisionDispatch\btDefaultCollisionConfiguration.h"
#include "BulletCollision\CollisionDispatch\btCollisionDispatcher.h"
#include "BulletDynamics\ConstraintSolver\btConstraintSolver.h"
#include "BulletDynamics\ConstraintSolver\btSequentialImpulseConstraintSolver.h"


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
	btBroadphaseInterface* btBroadphase				= Memory::Alloc<btDbvtBroadphase>();
	btDefaultCollisionConfiguration* btCollConfig	= Memory::Alloc<btDefaultCollisionConfiguration>();
	btCollisionDispatcher* btDispatcher				= Memory::Alloc<btCollisionDispatcher>( btCollConfig );
	btConstraintSolver* btSolver					= Memory::Alloc<btSequentialImpulseConstraintSolver>();

	btDynamicsWorld* btWorld = Memory::Alloc<btDiscreteDynamicsWorld>( 
		btDispatcher, btBroadphase, btSolver, btCollConfig 
	);


}


void PhysicsManagerImpl::Finalize()
{
	if ( !_isInitialized ) {
		return;
	}
}