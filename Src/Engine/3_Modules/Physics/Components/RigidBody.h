// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"

#include "../../../2_BuildingBlocks/Component.h"
#include "../../../2_BuildingBlocks/Math/Vector3.h"

namespace CYRED
{
	namespace Enum_CollisionShapeType
	{
		enum Enum
		{
			BOX,
			SPHERE
		};
	}
	typedef Enum_CollisionShapeType::Enum CollisionShapeType;
}


namespace CYRED
{
	class DLL RigidBody : public Component
	{
	public:
		RigidBody( GameObject* gameObject );
		virtual ~RigidBody();


	public:
		void OnAdded	()							override;
		void OnRemoved	()							override;
		void Clone		( Component* clone ) const	override;


	public:
		bool				IsTrigger		() const;
		void				SetIsTrigger	( bool value );

		CollisionShapeType	GetShapeType	() const;
		void				SetShapeType	( CollisionShapeType type );

		Vector3				GetShapeSize	() const;
		void				SetShapeSize	( const Vector3& size );

		float				GetMass			() const;
		void				SetMass			( float mass );


	private:
		void _OnEnable() override {}


	private:
		bool				_isTrigger;
		CollisionShapeType	_shapeType;
		Vector3				_shapeSize;
		float				_mass;
	};
}