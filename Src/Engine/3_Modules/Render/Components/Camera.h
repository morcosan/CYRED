// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"

#include "../../../2_BuildingBlocks/Component.h"
#include "../../../2_BuildingBlocks/Math/Math.h"


namespace CYRED
{
	namespace Enum_CameraType
	{
		enum Enum
		{
			PERSPECTIVE,
			ORTHOGRAPHIC
		};
	}
	typedef Enum_CameraType::Enum CameraType;
}


namespace CYRED
{
	namespace COMP
	{
		class DLL Camera : public Component
		{
		public:
			Camera( GameObject* gameObject = NULL );
			virtual ~Camera() {}


		public:
			void OnHierarchyChange()	override {}


		public:
			Matrix4 GetProjectionMatrix();

			// getters
			float			GetFovYAngle	()	const;
			float			GetNearClipping	()	const;
			float			GetFarClipping	()	const;
			float			GetAspectRatio	()	const;
			Vector2		GetOrthoSize	()	const;
			CameraType	GetType			()	const;

			// setters
			void SetFovYAngle		( float value );
			void SetNearClipping	( float value );
			void SetFarClipping	( float value );
			void SetAspectRatio	( float value );
			void SetOrthoSize		( Vector2& value );
			void SetType			( CameraType type );


		protected:
			void _OnEnable() override {}


		protected:
			CameraType	_type;
			float		_fovYAngle;
			float		_nearClipping;
			float		_farClipping;
			float		_aspectRatio;
			Vector2		_orthoSize;

			Matrix4		_projectionMatrix;
			bool		_projectionChanged;
		};
	}
}