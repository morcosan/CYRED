// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"

#include "../../../2_BuildingBlocks/Component.h"
#include "../../../2_BuildingBlocks/Math/Math.h"


namespace CYRED
{
	namespace Enum_LightType
	{
		enum Enum
		{
			DIRECTIONAL = 0
			, POINT
			, SPOT
		};
	}
	typedef Enum_LightType::Enum LightType;
}


namespace CYRED
{
	class DLL Light : public Component
	{
	public:
		static cchar* const TYPE_DIRECTIONAL;
		static cchar* const TYPE_POINT;
		static cchar* const TYPE_SPOT;


	public:
		Light( GameObject* gameObject = NULL );
		virtual ~Light() {}


	public:
		void Clone( Component* clone ) const override;


	public:
		LightType	GetLightType		() const;
		Vector3		GetColor			() const;
		float		GetIntensity		() const;
		float		GetRange			() const;
		float		GetSpotAngle		() const;
		cchar*	GetLightTypeString	() const;

		void SetLightType		( LightType value );
		void SetColor			( Vector3& value );
		void SetIntensity		( float value );
		void SetRange			( float value );
		void SetSpotAngle		( float value );
		void SetLightTypeString	( cchar* value );


	private:
		void _OnEnable() override {}


	private:
		LightType	_lightType;
		Vector3		_color;
		float		_intensity;
		float		_range;
		float		_spotAngle;
	};
}