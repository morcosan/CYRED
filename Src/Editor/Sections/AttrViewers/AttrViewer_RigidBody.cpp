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
		_target->SetEnabled( _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() );
	}
	_target->SetEmitEvents( TRUE );

	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, _target );
}


void AttrViewer_RigidBody::_ChangeVisibility()
{
	switch ( _target->GetShapeType() ) {
		case CollisionShapeType::BOX:
			//_SetAttrVisibility( ATTR_FILE_PATH, TRUE );
			break;

		case CollisionShapeType::SPHERE:
			//_SetAttrVisibility( ATTR_FILE_PATH, FALSE );
			break;
	}

	_UpdateVisibility();
}

