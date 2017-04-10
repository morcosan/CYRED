// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "AttrViewer_Light.h"
#include "CyredModule_Event.h"


using namespace CYRED;



AttrViewer_Light::AttrViewer_Light()
{
	_lightTypes.Add( "Directional" );
	_lightTypes.Add( "Point" );
	_lightTypes.Add( "Spot" );
}


void AttrViewer_Light::_OnInitialize()
{
	_CreateAttrDropdown	( ATTR_LIGHT_TYPE, ATTR_LIGHT_TYPE, _lightTypes, AttrFlag::NONE, CallbackGroup::GROUP_2 );

	_CreateAttrFloat	( ATTR_COLOR,		ATTR_COLOR );
	_CreateAttrFloat	( ATTR_INTENSITY,	ATTR_INTENSITY );
	_CreateAttrFloat	( ATTR_RANGE,		ATTR_RANGE );
	_CreateAttrFloat	( ATTR_SPOT_ANGLE,	ATTR_SPOT_ANGLE );

	_CreateInnerAttribute( InnerAttrType::ENABLED );

	_AddToPanel( TITLE );
}


void AttrViewer_Light::_OnChangeTarget( void* target )
{
	_target = CAST_S( Light*, target );
}


void AttrViewer_Light::_OnUpdateGUI()
{
	_WriteAttrDropdown( ATTR_LIGHT_TYPE, _GetIndexForType( _target->GetLightType() ) );
	_WriteAttrVector3( ATTR_COLOR,		_target->GetColor() );
	_WriteAttrFloat( ATTR_INTENSITY,	_target->GetIntensity() );
	_WriteAttrFloat( ATTR_RANGE,		_target->GetRange() );
	_WriteAttrFloat( ATTR_SPOT_ANGLE,	_target->GetSpotAngle() );

	if ( _target->IsEnabled() != _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() ) {
		DataUnion attr;
		_WriteInnerAttribute( InnerAttrType::ENABLED, attr.SetBool( _target->IsEnabled() ) );

		_Colorize( _target->IsEnabled() );
	}

	_ChangeVisibility();
}


void AttrViewer_Light::_OnUpdateTarget()
{
	_target->SetEmitEvents( FALSE );

	if ( _activatedGroup == CallbackGroup::GROUP_2 ) {
		_target->SetLightType( _GetTypeForIndex( _ReadAttrDropdown( ATTR_LIGHT_TYPE ) ) );

		_ChangeVisibility();
	}
	else {
		_target->SetColor		( _ReadAttrVector3( ATTR_COLOR ) );
		_target->SetIntensity	( _ReadAttrFloat( ATTR_INTENSITY ) );
		_target->SetRange		( _ReadAttrFloat( ATTR_RANGE ) );
		_target->SetSpotAngle	( _ReadAttrFloat( ATTR_SPOT_ANGLE ) );

		Bool newValue = _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool();
		if ( _target->IsEnabled() != newValue ) {
			_target->SetEnabled( newValue );

			_Colorize( _target->IsEnabled() );
		}
	}

	_target->SetEmitEvents( TRUE );

	// emit event manually
	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_CAMERA, _target );
}


Int AttrViewer_Light::_GetIndexForType( LightType type )
{
	switch ( type )	{
		case LightType::DIRECTIONAL:
			return 0;
		case LightType::POINT:
			return 1;
		case LightType::SPOT:
			return 2;
	}

	return -1;
}


LightType AttrViewer_Light::_GetTypeForIndex( Int index )
{
	switch ( index ) {
		case 0:
			return LightType::DIRECTIONAL;
		case 1:
			return LightType::POINT;
		case 2:
			return LightType::SPOT;
	}

	return LightType::DIRECTIONAL;
}


void AttrViewer_Light::_ChangeVisibility()
{
	switch ( _target->GetLightType() ) {
		case LightType::DIRECTIONAL:
			_SetAttrVisibility( ATTR_RANGE, FALSE );
			_SetAttrVisibility( ATTR_SPOT_ANGLE, FALSE );
			break;

		case LightType::POINT:
			_SetAttrVisibility( ATTR_RANGE, TRUE );
			_SetAttrVisibility( ATTR_SPOT_ANGLE, FALSE );
			break;

		case LightType::SPOT:
			_SetAttrVisibility( ATTR_RANGE, TRUE );
			_SetAttrVisibility( ATTR_SPOT_ANGLE, TRUE );
			break;
	}

	_UpdateVisibility();
}


