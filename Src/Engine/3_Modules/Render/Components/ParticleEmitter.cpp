// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "ParticleEmitter.h"
#include "../RenderManagerImpl.h"
#include "../Assets/Material.h"
#include "../../../2_BuildingBlocks/Math/Math.h"
#include "../../../2_BuildingBlocks/Random/Random.h"
#include "../../Time/TimeManager.h"


using namespace CYRED;


ParticleEmitter::ParticleEmitter( GameObject* gameObject )
	: Component( gameObject )
	, _vbo( EMPTY_BUFFER )
	, _ibo( EMPTY_BUFFER )
	, _numIndices( 0 )
	, _material( NULL )
	, _maxParticles( 1000 )
	, _particleSizeStart( Vector2( 0.5f, 0.5f ) )
	, _particleSizeEnd( Vector2( 0.5f, 0.5f ) )
	, _particleLifetime( 5.0f )
	, _particleVelocity( Vector3(0, 0, -1) )
	, _particleAcceleration( Vector3() )
	, _shapeDrivenSpeed( 0.0f )
	, _shapeDrivenAccel( 0.0f )
	, _wavesPerSec( 10.0f )
	, _particlesPerWave( 1 )
	, _isLooping( TRUE )
	, _spawnDuration( 5.0f )
	, _emitterShape( EmitterShape::CIRCLE )
	, _spawnFromEdge( FALSE )
	, _shapeRadius( 1.0f )
{
	_componentType = ComponentType::PARTICLE_EMITTER;
}


ParticleEmitter::~ParticleEmitter()
{
	NotAPI::RenderManagerImpl::Singleton()->DeleteBuffers( _vbo, _ibo );
}


void ParticleEmitter::BindToGPU()
{
	_particlesAge.Clear();

	//! we don't need all paticles
	UInt neededParticles = _isLooping ? CAST_S( UInt, _particleLifetime * _wavesPerSec ) :
										CAST_S( UInt, _spawnDuration * _wavesPerSec );
	neededParticles *= _particlesPerWave;
	neededParticles = (neededParticles > _maxParticles) ? _maxParticles : neededParticles;

	for ( UInt i = 0; i < neededParticles; ++i )
	{
		Vector3 position;
		Vector3 velocity;
		Vector3 acceleration;

		switch ( _emitterShape )
		{
			case EmitterShape::CIRCLE:
				{
					Vector2 circlePoint = _spawnFromEdge ? Random::FromCircleEdge( _shapeRadius ) :
															Random::FromCircle( _shapeRadius );
					position = Vector3( circlePoint.x, circlePoint.y, 0 );
					velocity = position * _shapeDrivenSpeed + _particleVelocity;
					acceleration = position * _shapeDrivenAccel + _particleAcceleration;
				}
				break;

			case EmitterShape::SPHERE:
				{
					position = _spawnFromEdge ? Random::FromSphereEdge( _shapeRadius ) :
												Random::FromSphere( _shapeRadius );
					velocity = position * _shapeDrivenSpeed + _particleVelocity;
					acceleration = position * _shapeDrivenAccel + _particleAcceleration;
				}
				break;
		}

		//! particles start with negative age
		Float age = - (1.0f / _wavesPerSec) * (i / _particlesPerWave);

		Vector2 size;
		size.x = Random::FromRange( _particleSizeStart.x, _particleSizeStart.y );
		size.y = Random::FromRange( _particleSizeEnd.x, _particleSizeEnd.y );

		_vertices.Add( ParticleVertex( position, velocity, acceleration, age, size ) );
		_indices.Add( i );

		_particlesAge.Add( -1.0f );
	}
	_numIndices = neededParticles;


	NotAPI::RenderManagerImpl::Singleton()->CreateParticleBuffers( _vbo, _ibo, _vertices, _indices );

	_vertices.Clear();
	_indices.Clear();
}


UInt ParticleEmitter::GetVBO() const
{
	return _vbo;
}


UInt ParticleEmitter::GetIBO() const
{
	return _ibo;
}


UInt ParticleEmitter::GetNumIndices() const
{
	return _numIndices;
}


Material* ParticleEmitter::GetMaterial() const
{
	return _material;
}


Vector2 ParticleEmitter::GetParticleSizeStart() const
{
	return _particleSizeStart;
}


Vector2 ParticleEmitter::GetParticleSizeEnd() const
{
	return _particleSizeEnd;
}


void ParticleEmitter::SetMaterial( Material* value )
{
	_material = value;
}


void ParticleEmitter::SetParticleSizeStart( Vector2 value )
{
	_particleSizeStart = value;
}


void ParticleEmitter::SetParticleSizeEnd( Vector2 value )
{
	_particleSizeEnd = value;
}


void ParticleEmitter::SetMaxParticles( UInt value )
{
	_maxParticles = value;
}


void ParticleEmitter::SetParticleLifetime( Float value )
{
	_particleLifetime = value;
}


void ParticleEmitter::SetParticleVelocity( Vector3 value )
{
	_particleVelocity = value;
}


void ParticleEmitter::SetParticleAccel( Vector3 value )
{
	_particleAcceleration = value;
}


void ParticleEmitter::SetShapeDrivenSpeed( Float value )
{
	_shapeDrivenSpeed = value;
}


void ParticleEmitter::SetShapeDrivenAccel( Float value )
{
	_shapeDrivenAccel = value;
}


UInt ParticleEmitter::GetMaxParticles() const
{
	return _maxParticles;
}


Float ParticleEmitter::GetParticleLifetime() const
{
	return _particleLifetime;
}


Vector3 ParticleEmitter::GetParticleVelocity() const
{
	return _particleVelocity;
}


Vector3 ParticleEmitter::GetParticleAccel() const
{
	return _particleAcceleration;
}


Float ParticleEmitter::GetShapeDrivenSpeed() const
{
	return _shapeDrivenSpeed;
}


Float ParticleEmitter::GetShapeDrivenAccel() const
{
	return _shapeDrivenAccel;
}


void ParticleEmitter::SetWavesPerSec( Float value )
{
	_wavesPerSec = value;
}


void ParticleEmitter::SetParticlesPerWave( UInt value )
{
	_particlesPerWave = value;
}


void ParticleEmitter::SetIsLooping( Bool value )
{
	_isLooping = value;
}


void ParticleEmitter::SetSpawnDuration( Float value )
{
	_spawnDuration = value;
}


Float ParticleEmitter::GetWavesPerSec() const
{
	return _wavesPerSec;
}


UInt ParticleEmitter::GetParticlesPerWave() const
{
	return _particlesPerWave;
}


Float ParticleEmitter::GetSpawnDuration() const
{
	return _spawnDuration;
}


Bool ParticleEmitter::IsLooping() const
{
	return _isLooping;
}


void ParticleEmitter::SetEmitterShape( EmitterShape value )
{
	_emitterShape = value;
}

void ParticleEmitter::SetSpawnFromEndge( Bool value )
{
	_spawnFromEdge = value;
}


void ParticleEmitter::SetShapeRadius( Float value )
{
	_shapeRadius = value;
}


EmitterShape ParticleEmitter::GetEmitterShape() const
{
	return _emitterShape;
}


Bool ParticleEmitter::DoesSpawnFromEdge() const
{
	return _spawnFromEdge;
}


Float ParticleEmitter::GetShapeRadius() const
{
	return _shapeRadius;
}
