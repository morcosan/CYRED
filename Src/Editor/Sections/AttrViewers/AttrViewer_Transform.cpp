// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Transform.h"
#include "CyredModule_Event.h"
#include "CyredBuildingBlocks.h"


using namespace CYRED;



void AttrViewer_Transform::_OnInitialize()
{
	_OpenGroup( GROUP_LOCAL );
	_CreateAttrVector3	( ATTR_LOCAL_POS,	ATTR_LOCAL_POS,		AttrFlag::NONE, CallbackGroup::GROUP_1 );
	_CreateAttrVector3	( ATTR_LOCAL_ROT,	ATTR_LOCAL_ROT,		AttrFlag::NONE, CallbackGroup::GROUP_1 );
	_CreateAttrVector3	( ATTR_LOCAL_SCALE, ATTR_LOCAL_SCALE,	AttrFlag::NONE, CallbackGroup::GROUP_1 );
	_CloseGroup();

	_OpenGroup( GROUP_WORLD );
	_CreateAttrVector3	( ATTR_WORLD_POS,	ATTR_WORLD_POS,		AttrFlag::NONE, CallbackGroup::GROUP_2 );
	_CreateAttrVector3	( ATTR_WORLD_ROT,	ATTR_WORLD_ROT,		AttrFlag::NONE, CallbackGroup::GROUP_2 );
	_CreateAttrVector3	( ATTR_WORLD_SCALE, ATTR_WORLD_SCALE,	AttrFlag::NONE, CallbackGroup::GROUP_2 );
	_CloseGroup();
	
	_CreateInnerAttribute( InnerAttrType::ENABLED );
	_CreateInnerAttribute( InnerAttrType::SETTINGS );

	_AddToPanel( TITLE );
}


void AttrViewer_Transform::_OnChangeTarget( void* target )
{
	_target = CAST_S( Transform*, target );

	// prepare settings
	DataUnion attr;
	_WriteInnerAttribute( InnerAttrType::SETTINGS, attr.SetReference( _target ) );
}


void AttrViewer_Transform::_UpdateGUI()
{
	if ( _activatedGroup != CallbackGroup::GROUP_1 ) { 
		_WriteAttrVector3( ATTR_LOCAL_POS,		_target->GetPositionLocal() );
		_WriteAttrVector3( ATTR_LOCAL_ROT,		_target->GetEulerRotationLocal() );
		_WriteAttrVector3( ATTR_LOCAL_SCALE,	_target->GetScaleLocal() );
	}

	if ( _activatedGroup != CallbackGroup::GROUP_2 ) { 
		_WriteAttrVector3( ATTR_WORLD_POS,		_target->GetPositionWorld() );
		_WriteAttrVector3( ATTR_WORLD_ROT,		_target->GetEulerRotationWorld() );
		_WriteAttrVector3( ATTR_WORLD_SCALE,	_target->GetScaleWorld() );
	}
	
	if ( _target->IsEnabled() != _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() ) {
		DataUnion attr;
		_WriteInnerAttribute( InnerAttrType::ENABLED, attr.SetBool( _target->IsEnabled() ) );
		// change color 
		_Colorize( _target->IsEnabled() );
	}
}


void AttrViewer_Transform::_UpdateTarget()
{
	_target->SetEmitEvents( FALSE );
	{
		if ( _activatedGroup == CallbackGroup::GROUP_1 ) { 
			_target->SetPositionLocal		( _ReadAttrVector3( ATTR_LOCAL_POS ) );
			_target->SetEulerRotationLocal	( _ReadAttrVector3( ATTR_LOCAL_ROT ) );
			_target->SetScaleLocal			( _ReadAttrVector3( ATTR_LOCAL_SCALE ) );
		}
		else if ( _activatedGroup == CallbackGroup::GROUP_2 ) {
			_target->SetPositionWorld		( _ReadAttrVector3( ATTR_WORLD_POS ) );
			_target->SetEulerRotationWorld	( _ReadAttrVector3( ATTR_WORLD_ROT ) );
			_target->SetScaleWorld			( _ReadAttrVector3( ATTR_WORLD_SCALE ) );
		}

		bool newValue = _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool();
		if ( _target->IsEnabled() != newValue ) {
			_target->SetEnabled( newValue );
			// change color for enable
			_Colorize( _target->IsEnabled() );
		}
	}
	_target->SetEmitEvents( TRUE );

	// emit event manually
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_TRANSFORM, _target );
}


