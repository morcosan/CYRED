// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "RigidBody.h"

#include "../../Event/EventManager.h"


using namespace CYRED;


RigidBody::RigidBody( GameObject * gameObject )
	: Component( gameObject )
	, _shapeType( CollisionShapeType::BOX )
	, _shapeSize( Vector3( 1, 1, 1 ) )
	, _mass( 0 )
{
	_componentType = ComponentType::RIGID_BODY;
}


void RigidBody::OnAdded()
{
}


void RigidBody::OnRemoved()
{
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


void RigidBody::SetShapeSize( Vector3 size )
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

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, this );
	}
}
