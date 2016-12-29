// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Camera.h"

#include "../../Event/EventManager.h"
#include "../../../2_BuildingBlocks/String/String.h"


using namespace CYRED;


const Char* const Camera::TYPE_PERSPECTIVE	= "PERSPECTIVE";
const Char* const Camera::TYPE_ORTHOGRAPHIC	= "ORTHOGRAPHIC";


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


const Char* Camera::GetCameraTypeString() const
{
	if ( _cameraType == CameraType::ORTHOGRAPHIC ) {
		return TYPE_ORTHOGRAPHIC;
	}
	else if ( _cameraType == CameraType::PERSPECTIVE ) {
		return TYPE_PERSPECTIVE;
	}

	return NULL;
}


void Camera::SetFovYAngle( Float value )
{
	_fovYAngle = value;
	_projectionChanged = true;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_CAMERA, this );
	}
}


void Camera::SetNearClipping( Float value )
{
	_nearClipping = value;
	_projectionChanged = true;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_CAMERA, this );
	}
}


void Camera::SetFarClipping( Float value )
{
	_farClipping = value;
	_projectionChanged = true;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_CAMERA, this );
	}
}


void Camera::SetAspectRatio( Float value )
{
	_aspectRatio = value;
	_projectionChanged = true;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_CAMERA, this );
	}
}


void CYRED::Camera::SetOrthoHeight( Float value )
{
	_orthoSize.y = value;

	_projectionChanged = true;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_CAMERA, this );
	}
}


void CYRED::Camera::SetOrthoWidth( Float value )
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


void Camera::SetCameraTypeString( const Char* type )
{
	String sType( type );

	if ( sType == TYPE_PERSPECTIVE ) {
		SetCameraType( CameraType::PERSPECTIVE );
	}
	else if ( sType == TYPE_ORTHOGRAPHIC ) {
		SetCameraType( CameraType::ORTHOGRAPHIC );
	}
}
