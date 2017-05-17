// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/Math/Math.h"


namespace CYRED
{
	struct ParticleVertex
	{
		Vector4 position;
		Vector4 velocity;
		Vector4 acceleration;
		Vector4 age_sizeS_sizeE;


		ParticleVertex( const Vector3& pos, const Vector3& vel, const Vector3& acc,
						float age, Vector2 size )
			: position( pos.x, pos.y, pos.z, 0 )
			, velocity( vel.x, vel.y, vel.z, 0 )
			, acceleration( acc.x, acc.y, acc.z, 0 )
			, age_sizeS_sizeE( age, size.x, size.y, 0 )
		{
		}
	};
}