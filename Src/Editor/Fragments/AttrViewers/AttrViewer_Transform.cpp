// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Transform.h"
#include "CyredModule_Event.h"
#include "CyredBuildingBlocks.h"


using namespace CYRED;



void AttrViewer_Transform::OnSelect_Target( void* target )
{
	_target = CAST_S( COMP::Transform*, target );
	
	_OpenGroup( GROUP_LOCAL );
	_CreateAttrVector3	( ATTR_LOCAL_POS,	AttrFlag::NONE, CallbackGroup::GROUP_1 );
	_CreateAttrVector3	( ATTR_LOCAL_ROT,	AttrFlag::NONE, CallbackGroup::GROUP_1 );
	_CreateAttrVector3	( ATTR_LOCAL_SCALE, AttrFlag::NONE, CallbackGroup::GROUP_1 );
	_CloseGroup();

	_OpenGroup( GROUP_WORLD );
	_CreateAttrVector3	( ATTR_WORLD_POS,	AttrFlag::NONE, CallbackGroup::GROUP_2 );
	_CreateAttrVector3	( ATTR_WORLD_ROT,	AttrFlag::NONE, CallbackGroup::GROUP_2 );
	_CreateAttrVector3	( ATTR_WORLD_SCALE, AttrFlag::NONE, CallbackGroup::GROUP_2 );
	_CloseGroup();
	
	_CreateInnerAttribute( InnerAttrType::ENABLED );
	
	_AddToPanel( TITLE );
}


void AttrViewer_Transform::_OnUpdate_GUI()
{
	DataUnion attr;
	
	_WriteAttribute( ATTR_LOCAL_POS,	attr.SetVector3( _target->GetPositionLocal() ) );
	_WriteAttribute( ATTR_LOCAL_ROT,	attr.SetVector3( _target->GetEulerRotationLocal() ) );
	_WriteAttribute( ATTR_LOCAL_SCALE,	attr.SetVector3( _target->GetScaleLocal() ) );
		
	_WriteAttribute( ATTR_WORLD_POS,	attr.SetVector3( _target->GetPositionWorld() ) );
	_WriteAttribute( ATTR_WORLD_ROT,	attr.SetVector3( _target->GetEulerRotationWorld() ) );
	_WriteAttribute( ATTR_WORLD_SCALE,	attr.SetVector3( _target->GetScaleWorld() ) );
	
	_WriteInnerAttribute( InnerAttrType::ENABLED, attr.SetBool( _target->IsEnabled() ) );

	_Colorize( _target->IsEnabled() );
}


void AttrViewer_Transform::_OnUpdate_Target()
{
	_target->SetEmitEvents( FALSE );

	{
		if ( _activatedGroup == CallbackGroup::GROUP_1 )
		{
			Vector3 newPosLocal		= _ReadAttribute( ATTR_LOCAL_POS ).GetVector3();
			Vector3 newRotLocal		= _ReadAttribute( ATTR_LOCAL_ROT ).GetVector3();
			Vector3 newScaleLocal	= _ReadAttribute( ATTR_LOCAL_SCALE ).GetVector3();

			_target->SetPositionLocal		( newPosLocal );
			_target->SetEulerRotationLocal	( newRotLocal );
			_target->SetScaleLocal			( newScaleLocal );
		}

		if ( _activatedGroup == CallbackGroup::GROUP_2 )
		{
			Vector3 newPosWorld		= _ReadAttribute( ATTR_WORLD_POS ).GetVector3();
			Vector3 newRotWorld		= _ReadAttribute( ATTR_WORLD_ROT ).GetVector3();
			Vector3 newScaleWorld	= _ReadAttribute( ATTR_WORLD_SCALE ).GetVector3();

			_target->SetPositionWorld		( newPosWorld );
			_target->SetEulerRotationWorld	( newRotWorld );
			_target->SetScaleWorld			( newScaleWorld );
		}
	}
	{
		Bool newValue = _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool();
		_target->SetEnabled( newValue );
	}

	_target->SetEmitEvents( TRUE );

	// emit event manually
	EventManager::Singleton()->EmitEvent( EventType::COMPONENT, 
										  EventName::TRANSFORM_CHANGED, _target );

	_Colorize( _target->IsEnabled() );
}


