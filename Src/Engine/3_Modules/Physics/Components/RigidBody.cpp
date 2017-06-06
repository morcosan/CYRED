// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "RigidBody.h"

#include "../../Event/EventManager.h"
#include "../PhysicsManagerImpl.h"


using namespace CYRED;


RigidBody::RigidBody( GameObject* gameObject )
	: Component( gameObject )
	, _shapeType( CollisionShapeType::BOX )
	, _shapeSize( Vector3( 0.5f, 0.5f, 0.5f ) )
	, _mass( 0 )
	, _isTrigger( FALSE )
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
	NonAPI::PhysicsManagerImpl::Singleton()->RegisterObject( GetGameObject() );
}


void RigidBody::OnRemoved()
{
	// update physics
	NonAPI::PhysicsManagerImpl::Singleton()->UnregisterObject( GetGameObject() );
}


void RigidBody::Clone( Component* clone ) const
{
}


bool RigidBody::IsTrigger() const
{
	return _isTrigger;
}


void RigidBody::SetIsTrigger( bool value )
{
	_isTrigger = value;

	// update physics
	NonAPI::PhysicsManagerImpl::Singleton()->UpdateRigidBody( this );

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, this );
	}
}


CollisionShapeType RigidBody::GetShapeType() const
{
	return _shapeType;
}


void RigidBody::SetShapeType( CollisionShapeType type )
{
	_shapeType = type;

	// update physics
	NonAPI::PhysicsManagerImpl::Singleton()->UpdateRigidBody( this );

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

	// update physics
	NonAPI::PhysicsManagerImpl::Singleton()->UpdateRigidBody( this );

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
	NonAPI::PhysicsManagerImpl::Singleton()->UpdateRigidBody( this );

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, this );
	}
}
