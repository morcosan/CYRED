// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#include "AttrViewer_Camera.h"
#include "CyredModule_Event.h"


using namespace CYRED;



AttrViewer_Camera::AttrViewer_Camera()
{
	_cameraTypes.Add( "Perspective" );
	_cameraTypes.Add( "Ortographic" );
}


void AttrViewer_Camera::_OnInitialize()
{
	_CreateAttrDropdown	( ATTR_TYPE, ATTR_TYPE, _cameraTypes, AttrFlag::NONE, CallbackGroup::GROUP_2 );
	
	_CreateAttrFloat	( ATTR_FOVY_ANGLE,		ATTR_FOVY_ANGLE );
	_CreateAttrFloat	( ATTR_ORTH_HEIGHT,		ATTR_ORTH_HEIGHT );
	_CreateAttrFloat	( ATTR_NEAR_CLIPPING,	ATTR_NEAR_CLIPPING );
	_CreateAttrFloat	( ATTR_FAR_CLIPPING,	ATTR_FAR_CLIPPING );
	
	_CreateInnerAttribute( InnerAttrType::ENABLED );
	_CreateInnerAttribute( InnerAttrType::SETTINGS );

	_AddToPanel( TITLE );
}


void AttrViewer_Camera::_OnChangeTarget( void* target )
{
	_target = CAST_S( Camera*, target );

	// prepare settings
	DataUnion attr;
	_WriteInnerAttribute( InnerAttrType::SETTINGS, attr.SetReference( _target ) );

	// change color for enable
	_Colorize( _target->IsEnabled() );
}


void AttrViewer_Camera::_UpdateGUI()
{
	_WriteAttrDropdown( ATTR_TYPE, _GetIndexForType( _target->GetCameraType() ) );
	_WriteAttrFloat( ATTR_FOVY_ANGLE, _target->GetFovYAngle() );
	_WriteAttrFloat( ATTR_NEAR_CLIPPING, _target->GetNearClipping() );
	_WriteAttrFloat( ATTR_FAR_CLIPPING, _target->GetFarClipping() );
	_WriteAttrFloat( ATTR_ORTH_HEIGHT, _target->GetOrthoSize().y );
	
	if ( _target->IsEnabled() != _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() ) {
		DataUnion attr;
		_WriteInnerAttribute( InnerAttrType::ENABLED, attr.SetBool( _target->IsEnabled() ) );
		// change color for enable
		_Colorize( _target->IsEnabled() );
	}

	_ChangeVisibility();
}


void AttrViewer_Camera::_UpdateTarget()
{
	_target->SetEmitEvents( FALSE );

	if ( _activatedGroup == CallbackGroup::GROUP_2 ) {
		_target->SetCameraType( _GetTypeForIndex( _ReadAttrDropdown( ATTR_TYPE ) ) );

		_ChangeVisibility();
	}
	else {
		_target->SetFovYAngle( _ReadAttrFloat( ATTR_FOVY_ANGLE ) );
		_target->SetNearClipping( _ReadAttrFloat( ATTR_NEAR_CLIPPING ) );
		_target->SetFarClipping( _ReadAttrFloat( ATTR_FAR_CLIPPING ) );
		_target->SetOrthoHeight( _ReadAttrFloat( ATTR_ORTH_HEIGHT ) );
		
		bool newValue = _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool();
		if ( _target->IsEnabled() != newValue ) {
			_target->SetEnabled( newValue );
			// change color for enable
			_Colorize( _target->IsEnabled() );
		}
	}

	_target->SetEmitEvents( TRUE );

	// emit event manually
	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, _target );
}


int AttrViewer_Camera::_GetIndexForType( CameraType type )
{
	switch ( type )
	{
		case CameraType::PERSPECTIVE:
			return 0;

		case CameraType::ORTHOGRAPHIC:
			return 1;
	}

	return -1;
}


CameraType AttrViewer_Camera::_GetTypeForIndex( int index )
{
	switch ( index )
	{
		case 0:
			return CameraType::PERSPECTIVE;

		case 1:
			return CameraType::ORTHOGRAPHIC;
	}

	return CameraType::PERSPECTIVE;
}


void AttrViewer_Camera::_ChangeVisibility()
{
	switch ( _target->GetCameraType() )
	{
		case CameraType::ORTHOGRAPHIC:
			_SetAttrVisibility( ATTR_ORTH_HEIGHT, TRUE );
			_SetAttrVisibility( ATTR_FOVY_ANGLE, FALSE );
			break;

		case CameraType::PERSPECTIVE:
			_SetAttrVisibility( ATTR_ORTH_HEIGHT, FALSE );
			_SetAttrVisibility( ATTR_FOVY_ANGLE, TRUE );
			break;
	}

	_UpdateVisibility();
}


