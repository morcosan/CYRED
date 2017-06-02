// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_RigidBody.h"

#include "CyredModule_Physics.h"
#include "CyredModule_Event.h"


using namespace CYRED;


void AttrViewer_RigidBody::_OnInitialize()
{
	// add attributes
	_CreateInnerAttribute( InnerAttrType::ENABLED );
	_CreateInnerAttribute( InnerAttrType::SETTINGS );

	DataArray<cchar*> shapeTypes;
	shapeTypes.Add( SHAPE_TYPE_BOX );
	shapeTypes.Add( SHAPE_TYPE_SPHERE );
	_CreateAttrDropdown	( ATTR_SHAPE_TYPE, ATTR_SHAPE_TYPE, shapeTypes, AttrFlag::NONE, CallbackGroup::GROUP_2 );

	_CreateAttrVector3	( ATTR_SHAPE_SIZE,		ATTR_SHAPE_SIZE );
	_CreateAttrFloat	( ATTR_SHAPE_RADIUS,	ATTR_SHAPE_RADIUS );
	_CreateAttrFloat	( ATTR_MASS,			ATTR_MASS );

	_AddToPanel( TITLE );
}


void AttrViewer_RigidBody::_OnChangeTarget( void* target )
{
	_target = CAST_S( RigidBody*, target );

	// prepare settings
	DataUnion attr;
	_WriteInnerAttribute( InnerAttrType::SETTINGS, attr.SetReference( _target ) );
}


void AttrViewer_RigidBody::_UpdateGUI()
{
	{
		int typeIndex = 0;
		switch ( _target->GetShapeType() ) {
			case CollisionShapeType::BOX:		typeIndex = 0;	break;
			case CollisionShapeType::SPHERE:	typeIndex = 1;	break;
		}
		_WriteAttrDropdown( ATTR_SHAPE_TYPE, typeIndex );
	}

	_WriteAttrVector3	( ATTR_SHAPE_SIZE,		_target->GetShapeSize() );
	_WriteAttrFloat		( ATTR_SHAPE_RADIUS,	_target->GetShapeSize().x );
	_WriteAttrFloat		( ATTR_MASS,			_target->GetMass() );

	if ( _target->IsEnabled() != _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() ) {
		DataUnion attr;
		_WriteInnerAttribute( InnerAttrType::ENABLED, attr.SetBool( _target->IsEnabled() ) );

		_Colorize( _target->IsEnabled() );
	}

	// change ui based on options
	_ChangeVisibility();
}


void AttrViewer_RigidBody::_UpdateTarget()
{
	_target->SetEmitEvents( FALSE );
	{
		if ( _activatedGroup == CallbackGroup::GROUP_2 ) {
			CollisionShapeType shapeType;
			int typeIndex = _ReadAttrDropdown( ATTR_SHAPE_TYPE );
			switch ( typeIndex ) {
				case 0:	shapeType = CollisionShapeType::BOX;	break;
				case 1:	shapeType = CollisionShapeType::SPHERE;	break;
			}
			_target->SetShapeType( shapeType );

			// change ui based on options
			_ChangeVisibility();
		}
		else {
			if ( _target->GetShapeType() == CollisionShapeType::SPHERE ) {
				float radius = _ReadAttrFloat( ATTR_SHAPE_RADIUS );
				_target->SetShapeSize( Vector3( radius, radius, radius ) );
			}
			else {
				_target->SetShapeSize( _ReadAttrVector3( ATTR_SHAPE_SIZE ) );
			}

			_target->SetMass( _ReadAttrFloat( ATTR_MASS ) );

			_target->SetEnabled( _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() );
		}
	}
	_target->SetEmitEvents( TRUE );

	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, _target );
}


void AttrViewer_RigidBody::_ChangeVisibility()
{
	switch ( _target->GetShapeType() ) {
		case CollisionShapeType::BOX:
			_SetAttrVisibility( ATTR_SHAPE_SIZE, TRUE );
			_SetAttrVisibility( ATTR_SHAPE_RADIUS, FALSE );
			break;

		case CollisionShapeType::SPHERE:
			_SetAttrVisibility( ATTR_SHAPE_SIZE, FALSE );
			_SetAttrVisibility( ATTR_SHAPE_RADIUS, TRUE );
			break;
	}

	_UpdateVisibility();
}

