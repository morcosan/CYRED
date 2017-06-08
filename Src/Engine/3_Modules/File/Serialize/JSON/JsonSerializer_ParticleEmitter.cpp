// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
		json.AddMember( rapidjson::StringRef( ATTR_ENABLED ),	
						emitter->IsEnabled(),	
						_al );
	}
	{
		json.AddMember( rapidjson::StringRef( ATTR_MAX_PARTICLES ), 
						emitter->GetMaxParticles(), 
						_al );
		json.AddMember( rapidjson::StringRef( ATTR_PARTICLE_LIFETIME ), 
						emitter->GetParticleLifetime(), 
						_al );
		json.AddMember( rapidjson::StringRef( ATTR_PARTICLE_VELOCITY ),
						_ToJsonVec3( emitter->GetParticleVelocity() ),
						_al );
		json.AddMember( rapidjson::StringRef( ATTR_PARTICLE_ACCEL ),
						_ToJsonVec3( emitter->GetParticleAccel() ),
						_al );
		json.AddMember( rapidjson::StringRef( ATTR_SHAPE_DRIVEN_SPEED ), 
						emitter->GetShapeDrivenSpeed(), 
						_al );
		json.AddMember( rapidjson::StringRef( ATTR_SHAPE_DRIVEN_ACCEL ), 
						emitter->GetShapeDrivenAccel(), 
						_al );
	}
	{
		json.AddMember( rapidjson::StringRef( ATTR_WAVES_PER_SEC ), 
						emitter->GetWavesPerSec(), 
						_al );
		json.AddMember( rapidjson::StringRef( ATTR_PARTICLES_PER_WAVE ), 
						emitter->GetParticlesPerWave(), 
						_al );
		json.AddMember( rapidjson::StringRef( ATTR_LOOPING ), 
						emitter->IsLooping(), 
						_al );
		json.AddMember( rapidjson::StringRef( ATTR_SPAWN_DURATION ), 
						emitter->GetSpawnDuration(), 
						_al );
	}
	{
		for ( int i = 0; i < _shapeEnums.Size(); ++i ) {
			if ( _shapeEnums[i] == emitter->GetEmitterShape() ) {
				rapidjson::Value stringNode;
				stringNode.SetString( _shapeStrings[i].GetChar(), _al );
				json.AddMember( rapidjson::StringRef( ATTR_EMITTER_SHAPE ), 
								stringNode, 
								_al );
				break;
			}
		}
		json.AddMember( rapidjson::StringRef( ATTR_SHAPE_RADIUS ), 
						emitter->GetShapeRadius(), 
						_al );
		json.AddMember( rapidjson::StringRef( ATTR_SPAWN_FROM_EDGE ), 
						emitter->DoesSpawnFromEdge(), 
						_al );
	}
	{
		{
			Material* material = emitter->GetMaterial();
			if ( material == NULL )	{
				rapidjson::Value nullNode;
				nullNode.SetNull();
				json.AddMember( rapidjson::StringRef( ATTR_MATERIAL ), nullNode, _al );
			}
			else {
				json.AddMember( rapidjson::StringRef( ATTR_MATERIAL ),
								rapidjson::StringRef( material->GetUniqueID() ),
								_al );
			}
		}
		json.AddMember( rapidjson::StringRef( ATTR_PARTICLE_SIZE_START ), 
						_ToJsonVec2( emitter->GetParticleSizeStart() ), 
						_al );
		json.AddMember( rapidjson::StringRef( ATTR_PARTICLE_SIZE_END ), 
						_ToJsonVec2( emitter->GetParticleSizeEnd() ), 
						_al );
	}

	return json;
}


void JsonSerializer_ParticleEmitter::FromJson( rapidjson::Value& json, OUT void* object,
											   DeserFlag flag )
{
	ParticleEmitter* emitter = CAST_S( ParticleEmitter*, object );

	bool emitEvents = emitter->DoesEmitEvents();
	emitter->SetEmitEvents( FALSE );


	if ( json.HasMember( ATTR_ENABLED ) ) {
		emitter->SetEnabled( json[ATTR_ENABLED].GetBool() );
	}

	if ( json.HasMember( ATTR_MAX_PARTICLES ) ) {
		emitter->SetMaxParticles( json[ATTR_MAX_PARTICLES].GetUint() );
	}

	if ( json.HasMember( ATTR_PARTICLE_LIFETIME ) ) {
		emitter->SetParticleLifetime( CAST_S( float, json[ATTR_PARTICLE_LIFETIME].GetDouble() ) );
	}

	if ( json.HasMember( ATTR_PARTICLE_VELOCITY ) ) {
		emitter->SetParticleVelocity( _FromJsonVec3( json[ATTR_PARTICLE_VELOCITY] ) );
	}

	if ( json.HasMember( ATTR_PARTICLE_ACCEL ) ) {
		emitter->SetParticleAccel( _FromJsonVec3( json[ATTR_PARTICLE_ACCEL] ) );
	}

	if ( json.HasMember( ATTR_SHAPE_DRIVEN_SPEED ) ) {
		emitter->SetShapeDrivenSpeed( CAST_S( float, json[ATTR_SHAPE_DRIVEN_SPEED].GetDouble() ) );
	}

	if ( json.HasMember( ATTR_SHAPE_DRIVEN_ACCEL ) ) {
		emitter->SetShapeDrivenAccel( CAST_S( float, json[ATTR_SHAPE_DRIVEN_ACCEL].GetDouble() ) );
	}

	if ( json.HasMember( ATTR_WAVES_PER_SEC ) ) {
		emitter->SetWavesPerSec( CAST_S( float, json[ATTR_WAVES_PER_SEC].GetDouble() ) );
	}

	if ( json.HasMember( ATTR_PARTICLES_PER_WAVE ) ) {
		emitter->SetParticlesPerWave( json[ATTR_PARTICLES_PER_WAVE].GetUint() );
	}

	if ( json.HasMember( ATTR_LOOPING ) ) {
		emitter->SetIsLooping( json[ATTR_LOOPING].GetBool() );
	}

	if ( json.HasMember( ATTR_SPAWN_DURATION ) ) {
		emitter->SetSpawnDuration( CAST_S( float, json[ATTR_SPAWN_DURATION].GetDouble() ) );
	}

	if ( json.HasMember( ATTR_EMITTER_SHAPE ) )
	{
		for ( int i = 0; i < _shapeEnums.Size(); ++i ) {
			cchar* value = json[ATTR_EMITTER_SHAPE].GetString();

			if ( _shapeStrings[i] == value ) {
				emitter->SetEmitterShape( _shapeEnums[i] );
				break;
			}
		}
	}

	if ( json.HasMember( ATTR_SHAPE_RADIUS ) ) {
		emitter->SetShapeRadius( CAST_S( float, json[ATTR_SHAPE_RADIUS].GetDouble() ) );
	}

	if ( json.HasMember( ATTR_SPAWN_FROM_EDGE ) ) {
		emitter->SetSpawnFromEndge( json[ATTR_SPAWN_FROM_EDGE].GetBool() );
	}

	if ( json.HasMember( ATTR_MATERIAL ) ) {
		if ( json[ATTR_MATERIAL].IsNull() ) {
			emitter->SetMaterial( NULL );
		}
		else {
			cchar* uniqueID = json[ATTR_MATERIAL].GetString();
			Material* material = AssetManager::Singleton()->GetMaterial( uniqueID );
			if ( material == NULL )	{
				bool isOk = Random::ValidateUniqueID( uniqueID );
				if ( isOk )	{
					material = new Material();
					material->SetUniqueID( uniqueID );
					AssetManager::Singleton()->AddMaterial( material );
				}
			}
			emitter->SetMaterial( material );
		}
	}

	if ( json.HasMember( ATTR_PARTICLE_SIZE_START ) ) {
		emitter->SetParticleSizeStart( _FromJsonVec2( json[ATTR_PARTICLE_SIZE_START] ) );
	}
	if ( json.HasMember( ATTR_PARTICLE_SIZE_END ) ) {
		emitter->SetParticleSizeEnd( _FromJsonVec2( json[ATTR_PARTICLE_SIZE_END] ) );
	}

	emitter->BindToGPU();


	emitter->SetEmitEvents( emitEvents );
}
