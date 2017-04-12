// Copyright (c) 2015 Morco (www.morco.ro)
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
		static const Char* const TYPE_DIRECTIONAL;
		static const Char* const TYPE_POINT;
		static const Char* const TYPE_SPOT;


	public:
		Light( GameObject* gameObject = NULL );
		virtual ~Light() {}


	public:
		void Clone( Component* clone ) const override;


	public:
		LightType	GetLightType		() const;
		Vector3		GetColor			() const;
		Float		GetIntensity		() const;
		Float		GetRange			() const;
		Float		GetSpotAngle		() const;
		const Char*	GetLightTypeString	() const;

		void SetLightType		( LightType value );
		void SetColor			( Vector3& value );
		void SetIntensity		( Float value );
		void SetRange			( Float value );
		void SetSpotAngle		( Float value );
		void SetLightTypeString	( const Char* value );


	protected:
		void _OnEnable() override {}


	protected:
		LightType	_lightType;
		Vector3		_color;
		Float		_intensity;
		Float		_range;
		Float		_spotAngle;
	};
}