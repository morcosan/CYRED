// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
	class DLL ParticleEmitter : public Component
	{
	public:
		ParticleEmitter( GameObject* gameObject = NULL );
		virtual ~ParticleEmitter();


	public:
		void Clone( Component* clone ) const override;


	public:
		void BindToGPU();

		int			GetVBO				() const;		
		int			GetIBO				() const;			
		int			GetNumIndices		() const;

		void			SetMaxParticles		( int value );
		void			SetParticleLifetime	( float value );
		void			SetParticleVelocity	( Vector3 value );
		void			SetParticleAccel	( Vector3 value );
		void			SetShapeDrivenSpeed	( float value );
		void			SetShapeDrivenAccel	( float value );

		int			GetMaxParticles		() const;
		float			GetParticleLifetime	() const;			
		Vector3			GetParticleVelocity	() const;			
		Vector3			GetParticleAccel	() const;			
		float			GetShapeDrivenSpeed	() const;
		float			GetShapeDrivenAccel	() const;

		void			SetWavesPerSec		( float value );
		void			SetParticlesPerWave	( int value );
		void			SetIsLooping		( bool value );
		void			SetSpawnDuration	( float value );

		float			GetWavesPerSec		() const;			
		int			GetParticlesPerWave	() const;			
		float			GetSpawnDuration	() const;	
		bool			IsLooping			() const;

		void			SetEmitterShape		( EmitterShape value );
		void			SetSpawnFromEndge	( bool value );
		void			SetShapeRadius		( float value );
			
		EmitterShape	GetEmitterShape		() const;	
		bool			DoesSpawnFromEdge	() const;			
		float			GetShapeRadius		() const;			

		void			SetMaterial			( Material* value );
		void			SetParticleSizeStart( Vector2 value );
		void			SetParticleSizeEnd	( Vector2 value );

		Material*		GetMaterial			() const;
		Vector2			GetParticleSizeStart() const;
		Vector2			GetParticleSizeEnd	() const;

			
	private:
		void _OnEnable() override {}


	private:
		uint		_vbo;
		uint		_ibo;
		int			_numIndices;

		DataArray<ParticleVertex>	_vertices;
		DataArray<int>				_indices;

		//! simulation
		int				_maxParticles;
		float			_particleLifetime;
		Vector3			_particleVelocity;
		Vector3			_particleAcceleration;
		float			_shapeDrivenSpeed;
		float			_shapeDrivenAccel;

		//! spawning
		float			_wavesPerSec;		
		int				_particlesPerWave;		
		bool			_isLooping;
		float			_spawnDuration;

		//! emission
		EmitterShape	_emitterShape;
		float			_shapeRadius;
		bool			_spawnFromEdge;

		//! rendering
		Material*		_material;
		Vector2			_particleSizeStart;
		Vector2			_particleSizeEnd;

		DataArray<float>	_particlesAge;
	};
}