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
	class DLL Camera : public Component
	{
	public:
		static const Char* const TYPE_PERSPECTIVE;
		static const Char* const TYPE_ORTHOGRAPHIC;


	public:
		Camera( GameObject* gameObject = NULL );
		virtual ~Camera() {}


	public:
		void Clone( Component* clone ) const override;


	public:
		Matrix4 GetProjectionMatrix();

		Float		GetFovYAngle		()	const;
		Float		GetNearClipping		()	const;
		Float		GetFarClipping		()	const;
		Float		GetAspectRatio		()	const;
		Vector2		GetOrthoSize		()	const;
		CameraType	GetCameraType		()	const;
		const Char*	GetCameraTypeString	()	const;

		void SetFovYAngle		( Float value );
		void SetNearClipping	( Float value );
		void SetFarClipping		( Float value );
		void SetAspectRatio		( Float value );
		void SetOrthoHeight		( Float value );
		void SetOrthoWidth		( Float value );
		void SetOrthoSize		( const Vector2& value );
		void SetCameraType		( CameraType type );
		void SetCameraTypeString( const Char* type );


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