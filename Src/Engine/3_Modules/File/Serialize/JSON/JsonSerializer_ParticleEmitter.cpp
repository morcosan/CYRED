// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_ParticleEmitter.h"
#include "../../../Render/Assets/Material.h"
#include "../../../Asset/AssetManager.h"
#include "../../../../2_BuildingBlocks/Random/Random.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



JsonSerializer_ParticleEmitter::JsonSerializer_ParticleEmitter()
{
	_shapeEnums.Add( EmitterShape::CIRCLE );
	_shapeStrings.Add( "CIRCLE" );

	_shapeEnums.Add( EmitterShape::SPHERE );
	_shapeStrings.Add( "SPHERE" );
}


rapidjson::Value JsonSerializer_ParticleEmitter::ToJson( const void* object )
{
	const ParticleEmitter* emitter = CAST_S( const ParticleEmitter*, object );

	rapidjson::Value json;
	json.SetObject();
	{
		json.AddMember( rapidjson::StringRef( ENABLED ),	
						emitter->IsEnabled(),	
						_al );
	}
	{
		json.AddMember( rapidjson::StringRef( MAX_PARTICLES ), 
						emitter->GetMaxParticles(), 
						_al );
		json.AddMember( rapidjson::StringRef( PARTICLE_LIFETIME ), 
						emitter->GetParticleLifetime(), 
						_al );
		json.AddMember( rapidjson::StringRef( PARTICLE_VELOCITY ),
						_ToJsonVec3( emitter->GetParticleVelocity() ),
						_al );
		json.AddMember( rapidjson::StringRef( PARTICLE_ACCEL ),
						_ToJsonVec3( emitter->GetParticleAccel() ),
						_al );
		json.AddMember( rapidjson::StringRef( SHAPE_DRIVEN_SPEED ), 
						emitter->GetShapeDrivenSpeed(), 
						_al );
		json.AddMember( rapidjson::StringRef( SHAPE_DRIVEN_ACCEL ), 
						emitter->GetShapeDrivenAccel(), 
						_al );
	}
	{
		json.AddMember( rapidjson::StringRef( WAVES_PER_SEC ), 
						emitter->GetWavesPerSec(), 
						_al );
		json.AddMember( rapidjson::StringRef( PARTICLES_PER_WAVE ), 
						emitter->GetParticlesPerWave(), 
						_al );
		json.AddMember( rapidjson::StringRef( LOOPING ), 
						emitter->IsLooping(), 
						_al );
		json.AddMember( rapidjson::StringRef( SPAWN_DURATION ), 
						emitter->GetSpawnDuration(), 
						_al );
	}
	{
		for ( UInt i = 0; i < _shapeEnums.Size(); ++i )
		{
			if ( _shapeEnums[i] == emitter->GetEmitterShape() )
			{
				rapidjson::Value stringNode;
				stringNode.SetString( _shapeStrings[i].GetChar(), _al );
				json.AddMember( rapidjson::StringRef( EMITTER_SHAPE ), 
								stringNode, 
								_al );
				break;
			}
		}
		json.AddMember( rapidjson::StringRef( SHAPE_RADIUS ), 
						emitter->GetShapeRadius(), 
						_al );
		json.AddMember( rapidjson::StringRef( SPAWN_FROM_EDGE ), 
						emitter->DoesSpawnFromEdge(), 
						_al );
	}
	{
		{
			Material* material = emitter->GetMaterial();
			if ( material == NULL )
			{
				rapidjson::Value nullNode;
				nullNode.SetNull();
				json.AddMember( rapidjson::StringRef( MATERIAL ), nullNode, _al );
			}
			else
			{
				json.AddMember( rapidjson::StringRef( MATERIAL ),
								rapidjson::StringRef( material->GetUniqueID() ),
								_al );
			}
		}
		json.AddMember( rapidjson::StringRef( PARTICLE_SIZE_START ), 
						_ToJsonVec2( emitter->GetParticleSizeStart() ), 
						_al );
		json.AddMember( rapidjson::StringRef( PARTICLE_SIZE_END ), 
						_ToJsonVec2( emitter->GetParticleSizeEnd() ), 
						_al );
	}

	return json;
}


void JsonSerializer_ParticleEmitter::FromJson( rapidjson::Value& json, OUT void* object,
											   DeserFlag flag )
{
	ParticleEmitter* emitter = CAST_S( ParticleEmitter*, object );

	Bool emitEvents = emitter->DoesEmitEvents();
	emitter->SetEmitEvents( FALSE );

	if ( json.HasMember( ENABLED ) )
	{
		emitter->SetEnabled( json[ENABLED].GetBool() );
	}

	if ( json.HasMember( MAX_PARTICLES ) )
	{
		emitter->SetMaxParticles( json[MAX_PARTICLES].GetUint() );
	}
	if ( json.HasMember( PARTICLE_LIFETIME ) )
	{
		emitter->SetParticleLifetime( CAST_S( Float, json[PARTICLE_LIFETIME].GetDouble() ) );
	}
	if ( json.HasMember( PARTICLE_VELOCITY ) )
	{
		emitter->SetParticleVelocity( _FromJsonVec3( json[PARTICLE_VELOCITY] ) );
	}
	if ( json.HasMember( PARTICLE_ACCEL ) )
	{
		emitter->SetParticleAccel( _FromJsonVec3( json[PARTICLE_ACCEL] ) );
	}
	if ( json.HasMember( SHAPE_DRIVEN_SPEED ) )
	{
		emitter->SetShapeDrivenSpeed( CAST_S( Float, json[SHAPE_DRIVEN_SPEED].GetDouble() ) );
	}
	if ( json.HasMember( SHAPE_DRIVEN_ACCEL ) )
	{
		emitter->SetShapeDrivenAccel( CAST_S( Float, json[SHAPE_DRIVEN_ACCEL].GetDouble() ) );
	}
	if ( json.HasMember( WAVES_PER_SEC ) )
	{
		emitter->SetWavesPerSec( CAST_S( Float, json[WAVES_PER_SEC].GetDouble() ) );
	}
	if ( json.HasMember( PARTICLES_PER_WAVE ) )
	{
		emitter->SetParticlesPerWave( json[PARTICLES_PER_WAVE].GetUint() );
	}
	if ( json.HasMember( LOOPING ) )
	{
		emitter->SetIsLooping( json[LOOPING].GetBool() );
	}
	if ( json.HasMember( SPAWN_DURATION ) )
	{
		emitter->SetSpawnDuration( CAST_S( Float, json[SPAWN_DURATION].GetDouble() ) );
	}

	if ( json.HasMember( EMITTER_SHAPE ) )
	{
		for ( UInt i = 0; i < _shapeEnums.Size(); ++i )
		{
			const Char* value = json[EMITTER_SHAPE].GetString();

			if ( _shapeStrings[i] == value )
			{
				emitter->SetEmitterShape( _shapeEnums[i] );
				break;
			}
		}
	}
	if ( json.HasMember( SHAPE_RADIUS ) )
	{
		emitter->SetShapeRadius( CAST_S( Float, json[SHAPE_RADIUS].GetDouble() ) );
	}
	if ( json.HasMember( SPAWN_FROM_EDGE ) )
	{
		emitter->SetSpawnFromEndge( json[SPAWN_FROM_EDGE].GetBool() );
	}

	if ( json.HasMember( MATERIAL ) )
	{
		if ( json[MATERIAL].IsNull() )
		{
			emitter->SetMaterial( NULL );
		}
		else
		{
			const Char* uniqueID = json[MATERIAL].GetString();
			Material* material = AssetManager::Singleton()->GetMaterial( uniqueID );
			if ( material == NULL )
			{
				Bool isOk = Random::ValidateUniqueID( uniqueID );
				if ( isOk )
				{
					material = Memory::Alloc<Material>();
					material->SetUniqueID( uniqueID );
					AssetManager::Singleton()->AddMaterial( material );
				}
			}
			emitter->SetMaterial( material );
		}
	}
	if ( json.HasMember( PARTICLE_SIZE_START ) )
	{
		emitter->SetParticleSizeStart( _FromJsonVec2( json[PARTICLE_SIZE_START] ) );
	}
	if ( json.HasMember( PARTICLE_SIZE_END ) )
	{
		emitter->SetParticleSizeEnd( _FromJsonVec2( json[PARTICLE_SIZE_END] ) );
	}

	emitter->BindToGPU();

	emitter->SetEmitEvents( emitEvents );
}
