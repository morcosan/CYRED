// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "../Component.h"
#include "../Math/Math.h"
#include "../Data/DataArray.h"


namespace CYRED
{
	namespace Enum_SpaceSystem
	{
		enum Enum
		{
			WORLD,
			LOCAL
		};
	}
	typedef Enum_SpaceSystem::Enum SpaceSystem;
}


namespace CYRED
{
	class DLL Transform : public Component
	{
	public:
		Transform( GameObject* gameObject );
		virtual ~Transform() {}


	public:
		void OnHierarchyChange()	override;


	public:
		void OnAdded	()							override {};
		void OnRemoved	()							override {};
		void Clone		( Component* clone ) const	override;


	public:
		// getters
		Matrix4		GetWorldMatrix			();
		Matrix4		GetViewMatrix			();

		Matrix4		GetPositionMatrix		();
		Matrix4		GetRotationMatrix		();
		Matrix4		GetScaleMatrix			();

		Vector3		GetPositionWorld		() const;
		Quaternion	GetRotationWorld		() const;
		Vector3		GetEulerRotationWorld	() const;
		Vector3		GetScaleWorld			() const;

		Vector3		GetPositionLocal		() const;
		Quaternion	GetRotationLocal		() const;
		Vector3		GetEulerRotationLocal	() const;
		Vector3		GetScaleLocal			() const;

		Vector3		GetLocalAxisX			() const;
		Vector3		GetLocalAxisY			() const;
		Vector3		GetLocalAxisZ			() const;
		Vector3		GetLocalForward			() const;

		// setters
		void SetPositionWorld		( const Vector3& value );
		void SetPositionLocal		( const Vector3& value );

		void SetRotationWorld		( const Quaternion& value );
		void SetRotationLocal		( const Quaternion& value );
		void SetEulerRotationWorld	( const Vector3& value );
		void SetEulerRotationLocal	( const Vector3& value );

		void SetScaleWorld			( const Vector3& value );
		void SetScaleLocal			( const Vector3& value );

		void TranslateByWorld		( const Vector3& value );
		void TranslateByLocal		( const Vector3& value );
		void Translate				( const Vector3& value, SpaceSystem spaceSystem );

		void RotateByWorld			( const Vector3& value );
		void RotateByLocal			( const Vector3& value );
		void Rotate					( const Vector3& value, SpaceSystem spaceSystem );

		void ScaleByConstant		( const Vector3& value );


	private:
		void _OnEnable() override;


	private:
		Vector3		_positionWorld;
		Quaternion	_rotationWorld;
		Vector3		_scaleWorld;

		Vector3		_positionLocal;
		Quaternion	_rotationLocal;
		Vector3		_scaleLocal;

		Vector3		_localAxisX;
		Vector3		_localAxisY;
		Vector3		_localAxisZ;

		Matrix4		_positionMatrix;
		Matrix4		_rotationMatrix;
		Matrix4		_scaleMatrix;

		Matrix4		_worldMatrix;
		Matrix4		_viewMatrix;

		bool		_positionHasChanged;
		bool		_rotationHasChanged;
		bool		_scaleHasChanged;
		bool		_worldHasChanged;
		bool		_viewHasChanged;

		Transform*				_parent;
		DataArray<Transform*>	_children;

		void _MarkPositionChanged();
		void _MarkRotationChanged();
		void _MarkScaleChanged();
		void _UpdateLocalAxis();

		void _RecalculatePositionWorld();
		void _RecalculateRotationWorld();
		void _RecalculateScaleWorld();

		void _RecursiveFindFirstParent();
		void _RecursiveFindFirstChildren( GameObject* currChildNode );
	};
}