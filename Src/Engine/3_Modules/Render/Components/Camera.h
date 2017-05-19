// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
		static const char* const TYPE_PERSPECTIVE;
		static const char* const TYPE_ORTHOGRAPHIC;


	public:
		Camera( GameObject* gameObject = NULL );
		virtual ~Camera() {}


	public:
		void Clone( Component* clone ) const override;


	public:
		Matrix4 GetProjectionMatrix();

		float		GetFovYAngle		()	const;
		float		GetNearClipping		()	const;
		float		GetFarClipping		()	const;
		float		GetAspectRatio		()	const;
		Vector2		GetOrthoSize		()	const;
		CameraType	GetCameraType		()	const;
		const char*	GetCameraTypeString	()	const;

		void SetFovYAngle		( float value );
		void SetNearClipping	( float value );
		void SetFarClipping		( float value );
		void SetAspectRatio		( float value );
		void SetOrthoHeight		( float value );
		void SetOrthoWidth		( float value );
		void SetOrthoSize		( const Vector2& value );
		void SetCameraType		( CameraType type );
		void SetCameraTypeString( const char* type );


	private:
		void _OnEnable() override {}


	private:
		CameraType	_cameraType;
		float		_fovYAngle;
		float		_nearClipping;
		float		_farClipping;
		float		_aspectRatio;
		Vector2		_orthoSize;

		Matrix4		_projectionMatrix;
		bool		_projectionChanged;
	};
}