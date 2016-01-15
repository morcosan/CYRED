// Copyright (c) 2015 Morco (www.morco.ro)
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
	{
		_CreateAttrDropdown	( ATTR_TYPE, _cameraTypes );
		_CreateAttrFloat	( ATTR_FOVY_ANGLE );
		_CreateAttrFloat	( ATTR_NEAR_CLIPPING );
		_CreateAttrFloat	( ATTR_FAR_CLIPPING );
		_CreateAttrFloat	( ATTR_ASPECT_RATIO );
		_CreateAttrVector2	( ATTR_ORTH_SIZE );
	}
	{
		_CreateInnerAttribute( InnerAttrType::ENABLED );
	}
	_AddToPanel( TITLE );
}


void AttrViewer_Camera::_OnChangeTarget( void* target )
{
	_target = CAST_S( COMP::Camera*, target );
}


void AttrViewer_Camera::_OnUpdateGUI()
{
	/*DataUnion attr;
	
	_WriteAttribute( ATTR_TYPE,				attr.SetInt( _GetIndexForType( _target->GetType() ) ) );
	_WriteAttribute( ATTR_FOVY_ANGLE,		attr.SetFloat( _target->GetFovYAngle() ) );
	_WriteAttribute( ATTR_NEAR_CLIPPING,	attr.SetFloat( _target->GetNearClipping() ) );
	_WriteAttribute( ATTR_FAR_CLIPPING,		attr.SetFloat( _target->GetFarClipping() ) );
	_WriteAttribute( ATTR_ASPECT_RATIO,		attr.SetFloat( _target->GetAspectRatio() ) );
	_WriteAttribute( ATTR_ORTH_SIZE,		attr.SetVector2( _target->GetOrthoSize() ) );
	
	_WriteInnerAttribute( InnerAttrType::ENABLED,		attr.SetBool( _target->IsEnabled() ) );

	_Colorize( _target->IsEnabled() );*/
}


void AttrViewer_Camera::_OnUpdateTarget()
{
	//_target->SetEmitEvents( FALSE );

	//{
	//	CameraType	newType			= _GetTypeForIndex( _ReadAttribute( ATTR_TYPE ).GetInt() );
	//	Float		newFovYAngle	= _ReadAttribute( ATTR_FOVY_ANGLE ).GetFloat();
	//	Float		newNearClipping	= _ReadAttribute( ATTR_NEAR_CLIPPING ).GetFloat();
	//	Float		newFarClipping	= _ReadAttribute( ATTR_FAR_CLIPPING ).GetFloat();
	//	Float		newAspectRatio	= _ReadAttribute( ATTR_ASPECT_RATIO ).GetFloat();
	//	Vector2		newOrthoSize	= _ReadAttribute( ATTR_ORTH_SIZE ).GetVector2();
	//		
	//	_target->SetType		( newType );
	//	_target->SetFovYAngle	( newFovYAngle );
	//	_target->SetNearClipping( newNearClipping );
	//	_target->SetFarClipping	( newFarClipping );
	//	_target->SetAspectRatio	( newAspectRatio );
	//	_target->SetOrthoSize	( newOrthoSize );
	//}
	//{
	//	Bool newValue = _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool();
	//	_target->SetEnabled( newValue );
	//}

	//_target->SetEmitEvents( TRUE );

	//// emit event manually
	//EventManager::Singleton()->EmitEvent( EventType::COMPONENT, 
	//									  EventName::CAMERA_CHANGED, _target );

	//_Colorize( _target->IsEnabled() );
}


Int AttrViewer_Camera::_GetIndexForType( CameraType type )
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


CameraType AttrViewer_Camera::_GetTypeForIndex( Int index )
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


