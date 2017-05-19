// Copyright (c) 2015-2017 Morco (www.morco.ro)
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


void ParticleEmitter::Clone( Component* clone ) const
{
}


void ParticleEmitter::BindToGPU()
{
	_particlesAge.Clear();

	//! we don't need all paticles
	int neededParticles = _isLooping ? CAST_S( int, _particleLifetime * _wavesPerSec ) :
										CAST_S( int, _spawnDuration * _wavesPerSec );
	neededParticles *= _particlesPerWave;
	neededParticles = (neededParticles > _maxParticles) ? _maxParticles : neededParticles;

	for ( int i = 0; i < neededParticles; ++i )
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
		float age = - (1.0f / _wavesPerSec) * (i / _particlesPerWave);

		Vector2 size;
		size.x = Random::FromRangeFloat( _particleSizeStart.x, _particleSizeStart.y );
		size.y = Random::FromRangeFloat( _particleSizeEnd.x, _particleSizeEnd.y );

		_vertices.Add( ParticleVertex( position, velocity, acceleration, age, size ) );
		_indices.Add( i );

		_particlesAge.Add( -1.0f );
	}
	_numIndices = neededParticles;


	NotAPI::RenderManagerImpl::Singleton()->CreateParticleBuffers( _vbo, _ibo, _vertices, _indices );

	_vertices.Clear();
	_indices.Clear();
}


int ParticleEmitter::GetVBO() const
{
	return _vbo;
}


int ParticleEmitter::GetIBO() const
{
	return _ibo;
}


int ParticleEmitter::GetNumIndices() const
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


void ParticleEmitter::SetMaxParticles( int value )
{
	_maxParticles = value;
}


void ParticleEmitter::SetParticleLifetime( float value )
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


void ParticleEmitter::SetShapeDrivenSpeed( float value )
{
	_shapeDrivenSpeed = value;
}


void ParticleEmitter::SetShapeDrivenAccel( float value )
{
	_shapeDrivenAccel = value;
}


int ParticleEmitter::GetMaxParticles() const
{
	return _maxParticles;
}


float ParticleEmitter::GetParticleLifetime() const
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


float ParticleEmitter::GetShapeDrivenSpeed() const
{
	return _shapeDrivenSpeed;
}


float ParticleEmitter::GetShapeDrivenAccel() const
{
	return _shapeDrivenAccel;
}


void ParticleEmitter::SetWavesPerSec( float value )
{
	_wavesPerSec = value;
}


void ParticleEmitter::SetParticlesPerWave( int value )
{
	_particlesPerWave = value;
}


void ParticleEmitter::SetIsLooping( bool value )
{
	_isLooping = value;
}


void ParticleEmitter::SetSpawnDuration( float value )
{
	_spawnDuration = value;
}


float ParticleEmitter::GetWavesPerSec() const
{
	return _wavesPerSec;
}


int ParticleEmitter::GetParticlesPerWave() const
{
	return _particlesPerWave;
}


float ParticleEmitter::GetSpawnDuration() const
{
	return _spawnDuration;
}


bool ParticleEmitter::IsLooping() const
{
	return _isLooping;
}


void ParticleEmitter::SetEmitterShape( EmitterShape value )
{
	_emitterShape = value;
}

void ParticleEmitter::SetSpawnFromEndge( bool value )
{
	_spawnFromEdge = value;
}


void ParticleEmitter::SetShapeRadius( float value )
{
	_shapeRadius = value;
}


EmitterShape ParticleEmitter::GetEmitterShape() const
{
	return _emitterShape;
}


bool ParticleEmitter::DoesSpawnFromEdge() const
{
	return _spawnFromEdge;
}


float ParticleEmitter::GetShapeRadius() const
{
	return _shapeRadius;
}
