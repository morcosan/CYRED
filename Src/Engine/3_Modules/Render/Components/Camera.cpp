// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Camera.h"

#include "../../Event/EventManager.h"


using namespace CYRED;
using namespace COMP;


Camera::Camera( GameObject * gameObject )
	: Component( gameObject )
	, _projectionChanged( true )
	, _type( CameraType::PERSPECTIVE )
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
		switch (_type)
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


CameraType Camera::GetType() const
{
	return _type;
}


void Camera::SetFovYAngle( float value )
{
	_fovYAngle = value;
	_projectionChanged = true;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT, EventName::CAMERA_CHANGED, this );
	}
}


void Camera::SetNearClipping( float value )
{
	_nearClipping = value;
	_projectionChanged = true;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT, EventName::CAMERA_CHANGED, this );
	}
}


void Camera::SetFarClipping( float value )
{
	_farClipping = value;
	_projectionChanged = true;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT, EventName::CAMERA_CHANGED, this );
	}
}


void Camera::SetAspectRatio( float value )
{
	_aspectRatio = value;
	_projectionChanged = true;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT, EventName::CAMERA_CHANGED, this );
	}
}


void Camera::SetOrthoSize( Vector2 & value )
{
	_orthoSize = value;
	_projectionChanged = true;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT, EventName::CAMERA_CHANGED, this );
	}
}


void Camera::SetType( CameraType type )
{
	_type = type;
	_projectionChanged = true;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT, EventName::CAMERA_CHANGED, this );
	}
}
