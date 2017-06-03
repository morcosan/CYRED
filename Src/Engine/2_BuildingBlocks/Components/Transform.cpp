// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Transform.h"

#include "../GameObject.h"
#include "../../3_Modules/Event/EventManager.h"


using namespace CYRED;


Transform::Transform( GameObject* gameObject ) 
	: Component( gameObject )
	, _positionHasChanged( true )
	, _rotationHasChanged( true )
	, _scaleHasChanged( true )
	, _worldHasChanged( true )
	, _viewHasChanged( true )
	, _positionWorld( Vector3() )
	, _rotationWorld( Quaternion() )
	, _scaleWorld( Vector3(1, 1, 1) )
	, _positionLocal( Vector3() )
	, _rotationLocal( Quaternion() )
	, _scaleLocal( Vector3(1, 1, 1) )
	, _localAxisX( Vector3(1, 0, 0) )
	, _localAxisY( Vector3(0, 1, 0) )
	, _localAxisZ( Vector3(0, 0, 1) )
	, _parent( NULL )
{	
	_componentType = ComponentType::TRANSFORM;
}


void Transform::_OnEnable()
{
	// update oneself
	OnHierarchyChange();

	// anounce both parent and children
	if ( _parent != NULL ) {
		_parent->OnHierarchyChange();
	}
	for ( int i = 0; i < _children.Size(); ++i ) {
		_children[i]->OnHierarchyChange();
	}
}


void Transform::OnHierarchyChange()
{
	Transform* lastParent = _parent;

	_RecursiveFindFirstParent();
	_RecursiveFindFirstChildren( _gameObject );

	// if it did change its parent
	if ( lastParent != _parent ) {
		if ( _parent != NULL ) {
			_positionLocal	= _positionWorld - _parent->GetPositionWorld();
			_rotationLocal	= _rotationWorld * Quaternion::Inverse( _parent->GetRotationWorld() );
			_scaleLocal		= _scaleWorld / _parent->GetScaleWorld();
		}
		else {
			_positionLocal	= _positionWorld;
			_rotationLocal	= _rotationWorld;
			_scaleLocal		= _scaleWorld;
		}
	}
}


void Transform::Clone( Component* clone ) const
{
	// cast to transform
	Transform* cloneComp = CAST_S( Transform*, clone );
	// set attributes
	cloneComp->SetPositionLocal( _positionLocal );
	cloneComp->SetRotationLocal( _rotationLocal );
	cloneComp->SetScaleLocal( _scaleLocal );
	cloneComp->SetEnabled( _enabled );
}


Vector3 Transform::GetEulerRotationWorld() const
{
	return Math::ToDegrees( _rotationWorld.ToEulerAngles() );
}


Vector3 Transform::GetEulerRotationLocal() const
{
	return Math::ToDegrees( _rotationLocal.ToEulerAngles() );
}


Matrix4 Transform::GetPositionMatrix()
{
	if ( _positionHasChanged ) {
		_positionMatrix = Matrix4::Translate( Matrix4::Identity(), _positionWorld );
		_positionHasChanged = false;
	}
	return _positionMatrix;
}


Matrix4 Transform::GetRotationMatrix()
{
	if ( _rotationHasChanged ) {
		_rotationMatrix =  _rotationWorld.ToMatrix4();
		_rotationHasChanged = false;
	}
	return _rotationMatrix;
}


Matrix4 Transform::GetScaleMatrix()
{
	if ( _scaleHasChanged ) {
		_scaleMatrix = Matrix4::Scale( Matrix4::Identity(), _scaleWorld );
		_scaleHasChanged = false;
	}
	return _scaleMatrix;
}


Matrix4 Transform::GetWorldMatrix()
{
	if ( _worldHasChanged )	{
		_worldMatrix = GetPositionMatrix() * GetRotationMatrix() * GetScaleMatrix();
		_worldHasChanged = false;
	}
	return _worldMatrix;
}


Matrix4 Transform::GetViewMatrix()
{
	if ( _viewHasChanged ) {
		_viewMatrix = Matrix4::Inverse( GetPositionMatrix() * GetRotationMatrix() );
		_viewHasChanged = false;
	}
	return _viewMatrix;
}


Vector3 Transform::GetPositionWorld() const
{
	return _positionWorld;
}


Quaternion Transform::GetRotationWorld() const
{
	return _rotationWorld;
}


Vector3 Transform::GetScaleWorld() const
{
	return _scaleWorld;
}


Vector3 Transform::GetPositionLocal() const
{
	return _positionLocal;
}


Quaternion Transform::GetRotationLocal() const
{
	return _rotationLocal;
}


Vector3 Transform::GetScaleLocal() const
{
	return _scaleLocal;
}


Vector3 Transform::GetLocalAxisX() const
{
	return _localAxisX;
}


Vector3 Transform::GetLocalAxisY() const
{
	return _localAxisX;
}


Vector3 Transform::GetLocalAxisZ() const
{
	return _localAxisZ;
}


Vector3 Transform::GetLocalForward() const
{
	return - _localAxisZ;
}


void Transform::_MarkPositionChanged()
{
	_positionHasChanged = true;
	_worldHasChanged = true;
	_viewHasChanged = true;
}


void Transform::_MarkRotationChanged()
{
	_rotationHasChanged = true;
	_worldHasChanged = true;
	_viewHasChanged = true;
}


void Transform::_MarkScaleChanged()
{
	_scaleHasChanged = true;
	_worldHasChanged = true;
}


void Transform::_UpdateLocalAxis()
{
	_localAxisX = _rotationWorld.ApplyRotation( Vector3( 1, 0, 0 ) );
	_localAxisY = _rotationWorld.ApplyRotation( Vector3( 0, 1, 0 ) );
	_localAxisZ = _rotationWorld.ApplyRotation( Vector3( 0, 0, 1 ) );
}


void Transform::SetPositionWorld( const Vector3& value )
{
	Vector3 diff = value - _positionWorld;
	SetPositionLocal( _positionLocal + diff );
}


void Transform::SetPositionLocal( const Vector3& value )
{
	_positionLocal = value;
	_RecalculatePositionWorld();

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, this );
	}
}


void Transform::TranslateByWorld( const Vector3& value )
{
	SetPositionWorld( _positionWorld + value );
}


void Transform::TranslateByLocal( const Vector3& value )
{
	Vector3 worldValue = _localAxisX * value.x + _localAxisY * value.y + _localAxisZ * value.z;
	SetPositionWorld( _positionWorld + worldValue );
}


void Transform::Translate( const Vector3& value, SpaceSystem spaceSystem )
{
	switch ( spaceSystem ) {
		case SpaceSystem::WORLD:
			TranslateByWorld( value );
			return;
		case SpaceSystem::LOCAL:
			TranslateByLocal( value );
			return;
	}
}


void Transform::SetRotationWorld( const Quaternion& value )
{
	Quaternion diff = Quaternion::Inverse( _rotationWorld ) * value; // order matters
	SetRotationLocal( _rotationLocal * diff );
}


void Transform::SetRotationLocal( const Quaternion& value )
{
	_rotationLocal = Quaternion::Normalize( value );
	_RecalculateRotationWorld();

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, this );
	}
}


void Transform::SetEulerRotationWorld( const Vector3& value )
{
	SetRotationWorld( Quaternion( Math::ToRadians( value ) ) );
}


void Transform::SetEulerRotationLocal( const Vector3& value )
{
	SetRotationLocal( Quaternion( Math::ToRadians( value ) ) );
}


void Transform::RotateByWorld( const Vector3& value )
{
	Quaternion rotate = Quaternion( Math::ToRadians( value ) );
	SetRotationWorld( rotate * _rotationWorld );
}


void Transform::RotateByLocal( const Vector3& value )
{
	Vector3 valueWorld = _localAxisX * value.x + 
						 _localAxisY * value.y + 
						 _localAxisZ * value.z;
	Quaternion rotate = Quaternion( Math::ToRadians( valueWorld ) );
	SetRotationWorld( rotate * _rotationWorld );
}


void Transform::Rotate( const Vector3& value, SpaceSystem spaceSystem )
{
	switch ( spaceSystem ) {
		case SpaceSystem::WORLD:
			RotateByWorld( value );
			return;
		case SpaceSystem::LOCAL:
			RotateByLocal( value );
			return;
	}
}


void Transform::SetScaleWorld( const Vector3& value )
{
	Vector3 newScale = value;

	// calculate newScale only if _scaleWorld not 0
	if ( _scaleWorld.x > 0 || _scaleWorld.x < 0 ) {
		newScale.x /= _scaleWorld.x;
	}
	if ( _scaleWorld.y > 0 || _scaleWorld.y < 0 ) {
		newScale.y /= _scaleWorld.y;
	}
	if ( _scaleWorld.z > 0 || _scaleWorld.z < 0 ) {
		newScale.z /= _scaleWorld.z;
	}

	ScaleByLocal( newScale );
}


void Transform::SetScaleLocal( const Vector3& value )
{
	_scaleLocal = value;
	_RecalculateScaleWorld();

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, this );
	}
}


void Transform::ScaleByWorld( const Vector3& value )
{
	Vector3 newScale = value;

	// calculate newScale only if _scaleWorld not 0
	if ( _scaleWorld.x > 0 || _scaleWorld.x < 0 ) {
		newScale.x *= _scaleWorld.x;
	}
	if ( _scaleWorld.y > 0 || _scaleWorld.y < 0 ) {
		newScale.y *= _scaleWorld.y;
	}
	if ( _scaleWorld.z > 0 || _scaleWorld.z < 0 ) {
		newScale.z *= _scaleWorld.z;
	}

	SetScaleWorld( newScale );
}


void Transform::ScaleByLocal( const Vector3& value )
{
	Vector3 newScale = value;

	// calculate newScale only if _scaleLocal not 0
	if ( _scaleLocal.x > 0 || _scaleLocal.x < 0 ) {
		newScale.x *= _scaleLocal.x;
	}
	if ( _scaleLocal.y > 0 || _scaleLocal.y < 0 ) {
		newScale.y *= _scaleLocal.y;
	}
	if ( _scaleLocal.z > 0 || _scaleLocal.z < 0 ) {
		newScale.z *= _scaleLocal.z;
	}

	SetScaleLocal( newScale );
}


void Transform::ScaleWithAdd( const Vector3& value )
{
	SetScaleLocal( _scaleLocal + value );
}


void Transform::_RecalculatePositionWorld()
{
	_MarkPositionChanged();

	if ( _parent != NULL ) {
		_positionWorld = _parent->GetRotationWorld().ApplyRotation( _positionLocal );
		_positionWorld += _parent->GetPositionWorld();
	}
	else {
		_positionWorld = _positionLocal;
	}

	for ( int i = 0; i < _children.Size(); ++i ) {
		_children[i]->_RecalculatePositionWorld();
	}
}


void Transform::_RecalculateRotationWorld()
{
	_MarkRotationChanged();

	if ( _parent != NULL ) {
		_rotationWorld = Quaternion::Normalize( _parent->GetRotationWorld() * _rotationLocal );
	}
	else {
		_rotationWorld = _rotationLocal;
	}

	for ( int i = 0; i < _children.Size(); ++i ) {
		_children[i]->_RecalculateRotationWorld();
		_children[i]->_RecalculatePositionWorld();
	}

	_UpdateLocalAxis();
}


void Transform::_RecalculateScaleWorld()
{
	_MarkScaleChanged();

	if ( _parent != NULL ) {
		_scaleWorld = _scaleLocal * _parent->GetScaleWorld();
	}
	else {
		_scaleWorld = _scaleLocal;
	}

	for ( int i = 0; i < _children.Size(); ++i ) {
		_children[i]->_RecalculateScaleWorld();
	}
}


void Transform::_RecursiveFindFirstParent()
{
	GameObject* parentNode = dynamic_cast<GameObject*>( _gameObject->GetParentNode() );
	Transform* parentTran = NULL;

	// try first parent
	if ( parentNode != NULL ) {
		parentTran = parentNode->GetComponent<Transform>();
		if ( parentTran != NULL ) {
			parentTran = parentTran->IsEnabled() ? parentTran : NULL;
		}
	}

	// go recursively till you find a parent
	while ( parentNode != NULL && parentTran == NULL ) {
		parentNode = dynamic_cast<GameObject*>( parentNode->GetParentNode() );

		if ( parentNode != NULL ) {
			parentTran = parentNode->GetComponent<Transform>();
			if ( parentTran != NULL ) {
				parentTran = parentTran->IsEnabled() ? parentTran : NULL;
			}
		}
	}

	_parent = parentTran;
}


void Transform::_RecursiveFindFirstChildren( GameObject* currChildNode )
{
	_children.Clear();

	if ( !_enabled ) {
		return;		// if we are disabled, we have no children, but still have parent
	}


	for ( int i = 0; i < currChildNode->GetChildNodeCount(); ++i ) {
		GameObject* childNode = CAST_S( GameObject*, currChildNode->GetChildNodeAt(i) );
		Transform* childTran = childNode->GetComponent<Transform>();

		if ( childTran != NULL ) {
			// we add it anyway
			_children.Add( childTran );

			// is not enabled, we take its children also
			if ( !childTran->IsEnabled() ) {
				_RecursiveFindFirstChildren( childNode );
			}
		}
	}
}

