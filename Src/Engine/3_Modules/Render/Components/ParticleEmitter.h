// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"

#include "../../../2_BuildingBlocks/Component.h"
#include "../../../2_BuildingBlocks/Data/DataArray.h"
#include "../OpenGL/ParticleVertex.h"


namespace CYRED
{
	class Material;

	namespace Enum_EmitterShape
	{
		enum Enum
		{
			CIRCLE
			, SPHERE
		};
	}
	typedef Enum_EmitterShape::Enum		EmitterShape;
}


namespace CYRED
{
	namespace COMP
	{
		class DLL ParticleEmitter : public Component
		{
		public:
			ParticleEmitter( GameObject* gameObject = NULL );
			virtual ~ParticleEmitter();


		public:
			void BindToGPU();

			UInt			GetVBO				() const;		
			UInt			GetIBO				() const;			
			UInt			GetNumIndices		() const;

			void			SetMaxParticles		( UInt value );
			void			SetParticleLifetime	( Float value );
			void			SetParticleVelocity	( Vector3 value );
			void			SetParticleAccel	( Vector3 value );
			void			SetShapeDrivenSpeed	( Float value );
			void			SetShapeDrivenAccel	( Float value );

			UInt			GetMaxParticles		() const;
			Float			GetParticleLifetime	() const;			
			Vector3			GetParticleVelocity	() const;			
			Vector3			GetParticleAccel	() const;			
			Float			GetShapeDrivenSpeed	() const;
			Float			GetShapeDrivenAccel	() const;

			void			SetWavesPerSec		( Float value );
			void			SetParticlesPerWave	( UInt value );
			void			SetIsLooping		( Bool value );
			void			SetSpawnDuration	( Float value );

			Float			GetWavesPerSec		() const;			
			UInt			GetParticlesPerWave	() const;			
			Float			GetSpawnDuration	() const;	
			Bool			IsLooping			() const;

			void			SetEmitterShape		( EmitterShape value );
			void			SetSpawnFromEndge	( Bool value );
			void			SetShapeRadius		( Float value );
			
			EmitterShape	GetEmitterShape		() const;	
			Bool			DoesSpawnFromEdge	() const;			
			Float			GetShapeRadius		() const;			

			void			SetMaterial			( Material* value );
			void			SetParticleSizeStart( Vector2 value );
			void			SetParticleSizeEnd	( Vector2 value );

			Material*		GetMaterial			() const;
			Vector2			GetParticleSizeStart() const;
			Vector2			GetParticleSizeEnd	() const;

			
		protected:
			void _OnEnable() override {}


		protected:
			UInt			_vbo;
			UInt			_ibo;
			UInt			_numIndices;

			DataArray<ParticleVertex>	_vertices;
			DataArray<UInt>				_indices;

			//! simulation
			UInt			_maxParticles;
			Float			_particleLifetime;
			Vector3			_particleVelocity;
			Vector3			_particleAcceleration;
			Float			_shapeDrivenSpeed;
			Float			_shapeDrivenAccel;

			//! spawning
			Float			_wavesPerSec;		
			UInt			_particlesPerWave;		
			Bool			_isLooping;
			Float			_spawnDuration;

			//! emission
			EmitterShape	_emitterShape;
			Float			_shapeRadius;
			Bool			_spawnFromEdge;

			//! rendering
			Material*		_material;
			Vector2			_particleSizeStart;
			Vector2			_particleSizeEnd;

			DataArray<Float>	_particlesAge;
		};
	}
}