// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Camera.h"

#include "../../Event/EventManager.h"
#include "../../../2_BuildingBlocks/String/String.h"


using namespace CYRED;


cchar* const Camera::TYPE_PERSPECTIVE	= "PERSPECTIVE";
cchar* const Camera::TYPE_ORTHOGRAPHIC	= "ORTHOGRAPHIC";


Camera::Camera( GameObject * gameObject )
	: Component( gameObject )
	, _projectionChanged( true )
	, _cameraType( CameraType::PERSPECTIVE )
	, _fovYAngle( 0 )
	, _nearClipping( 0 )
	, _farClipping( 0 )
	, _aspectRatio( 0 )
{
	_componentType = ComponentType::CAMERA;
}


void Camera::Clone( Component* clone ) const
{
}


Matrix4 Camera::GetProjectionMatrix()
{
	if (_projectionChanged)
	{
		switch (_cameraType)
		{
		case CameraType::PERSPECTIVE:
			_projectionMatrix = Matrix4::CreatePerspective( _fovYAngle, _aspectRatio, _nearClipping, _farClipping );
			break;

		case CameraType::ORTHOGRAPHIC:
			_projectionMatrix = Matrix4::CreateOrthogonal( _orthoSize, _nearClipping, _farClipping );
			break;
		}
		_projectionChanged = false;
	}

	return Matrix4( _projectionMatrix );
}


float Camera::GetFovYAngle() const
{
	return _fovYAngle;
}


float Camera::GetNearClipping() const
{
	return _nearClipping;
}


float Camera::GetFarClipping() const
{
	return _farClipping;
}


float Camera::GetAspectRatio() const
{
	return _aspectRatio;
}


Vector2 Camera::GetOrthoSize() const
{
	return _orthoSize;
}


CameraType Camera::GetCameraType() const
{
	return _cameraType;
}


cchar* Camera::GetCameraTypeString() const
{
	if ( _cameraType == CameraType::ORTHOGRAPHIC ) {
		return TYPE_ORTHOGRAPHIC;
	}
	else if ( _cameraType == CameraType::PERSPECTIVE ) {
		return TYPE_PERSPECTIVE;
	}

	return NULL;
}


void Camera::SetFovYAngle( float value )
{
	_fovYAngle = value;
	_projectionChanged = true;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_CAMERA, this );
	}
}


void Camera::SetNearClipping( float value )
{
	_nearClipping = value;
	_projectionChanged = true;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_CAMERA, this );
	}
}


void Camera::SetFarClipping( float value )
{
	_farClipping = value;
	_projectionChanged = true;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_CAMERA, this );
	}
}


void Camera::SetAspectRatio( float value )
{
	_aspectRatio = value;
	_projectionChanged = true;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_CAMERA, this );
	}
}


void CYRED::Camera::SetOrthoHeight( float value )
{
	_orthoSize.y = value;

	_projectionChanged = true;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_CAMERA, this );
	}
}


void CYRED::Camera::SetOrthoWidth( float value )
{
	_orthoSize.x = value;

	_projectionChanged = true;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_CAMERA, this );
	}
}


void Camera::SetOrthoSize( const Vector2& value )
{
	_orthoSize = value;
}


void Camera::SetCameraType( CameraType type )
{
	_cameraType = type;
	_projectionChanged = true;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_CAMERA, this );
	}
}


void Camera::SetCameraTypeString( cchar* type )
{
	String sType( type );

	if ( sType == TYPE_PERSPECTIVE ) {
		SetCameraType( CameraType::PERSPECTIVE );
	}
	else if ( sType == TYPE_ORTHOGRAPHIC ) {
		SetCameraType( CameraType::ORTHOGRAPHIC );
	}
}
