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
			Matrix4 GetProjectionMatrix();

			Float		GetFovYAngle	()	const;
			Float		GetNearClipping	()	const;
			Float		GetFarClipping	()	const;
			Float		GetAspectRatio	()	const;
			Vector2		GetOrthoSize	()	const;
			CameraType	GetCameraType	()	const;

			void SetFovYAngle	( Float value );
			void SetNearClipping( Float value );
			void SetFarClipping	( Float value );
			void SetAspectRatio	( Float value );
			void SetOrthoHeight	( Float value );
			void SetOrthoWidth	( Float value );
			void SetCameraType	( CameraType type );


		protected:
			void _OnEnable() override {}


		protected:
			CameraType	_cameraType;
			Float		_fovYAngle;
			Float		_nearClipping;
			Float		_farClipping;
			Float		_aspectRatio;
			Vector2		_orthoSize;

			Matrix4		_projectionMatrix;
			Bool		_projectionChanged;
		};
	}
}