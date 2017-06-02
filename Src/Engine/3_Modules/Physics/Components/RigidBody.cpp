// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "RigidBody.h"

#include "../../Event/EventManager.h"
#include "../PhysicsManager.h"


using namespace CYRED;


RigidBody::RigidBody( GameObject* gameObject )
	: Component( gameObject )
	, _shapeType( CollisionShapeType::BOX )
	, _shapeSize( Vector3( 0.5f, 0.5f, 0.5f ) )
	, _mass( 0 )
{
	_componentType = ComponentType::RIGID_BODY;
}


RigidBody::~RigidBody()
{
	// call on remove
	OnRemoved();
}


void RigidBody::OnAdded()
{
	// update physics
	PhysicsManager::Singleton()->RegisterRigidBody( this );
}


void RigidBody::OnRemoved()
{
	// update physics
	PhysicsManager::Singleton()->UnregisterRigidBody( this );
}


void RigidBody::Clone( Component* clone ) const
{
}


CollisionShapeType RigidBody::GetShapeType() const
{
	return _shapeType;
}


void RigidBody::SetShapeType( CollisionShapeType type )
{
	_shapeType = type;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, this );
	}
}


Vector3 RigidBody::GetShapeSize() const
{
	return _shapeSize;
}


void RigidBody::SetShapeSize( const Vector3& size )
{
	_shapeSize = size;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, this );
	}
}


float RigidBody::GetMass() const
{
	return _mass;
}


void RigidBody::SetMass( float mass )
{
	_mass = mass;

	// update physics
	PhysicsManager::Singleton()->UpdateRigidBody( this );

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, this );
	}
}
