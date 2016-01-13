// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "AttrViewer_ParticleEmitter.h"
#include "CyredModule_Render.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Event.h"
#include "../Selectors/Selector_Material.h"

using namespace CYRED;



AttrViewer_ParticleEmitter::AttrViewer_ParticleEmitter()
{
	_emitterShapes.Add( "Circle" );
	_emitterShapes.Add( "Sphere" );
}


void AttrViewer_ParticleEmitter::OnSelect_Target( void* target )
{
	_target = CAST_S( COMP::ParticleEmitter*, target );
	
	_OpenGroup( GROUP_PARTICLES );
	_CreateAttrInt		( ATTR_MAX_PARTICLES );
	_CreateAttrFloat	( ATTR_PARTICLE_LIFETIME );
	_CreateAttrVector3	( ATTR_PARTICLE_VELOCITY );
	_CreateAttrVector3	( ATTR_PARTICLE_ACCEL );
	_CreateAttrFloat	( ATTR_SHAPE_DRIVEN_SPEED );
	_CreateAttrFloat	( ATTR_SHAPE_DRIVEN_ACCEL );
	_CloseGroup();

	_OpenGroup( GROUP_SPAWNING );
	_CreateAttrFloat	( ATTR_WAVES_PER_SEC );
	_CreateAttrInt		( ATTR_PARTICLES_PER_WAVE );
	_CreateAttrBool		( ATTR_IS_LOOPING );
	_CreateAttrFloat	( ATTR_SPAWN_DURATION );
	_CloseGroup();

	_OpenGroup( GROUP_EMISSION );
	_CreateAttrDropdown	( ATTR_EMITTER_SHAPE, _emitterShapes );
	_CreateAttrFloat	( ATTR_SHAPE_RADIUS );
	_CreateAttrBool		( ATTR_SPAWN_FROM_EDGE );
	_CloseGroup();

	_OpenGroup( GROUP_RENDERING );
	_CreateAttrVector2	( ATTR_SIZE_START );
	_CreateAttrVector2	( ATTR_SIZE_END );
	_CreateAttrSelector	( ATTR_MATERIAL, Selector_Material::TYPE );
	_CloseGroup();
	
	_CreateInnerAttribute( InnerAttrType::ENABLED );
	
	_AddToPanel( TITLE );
}


void AttrViewer_ParticleEmitter::_OnUpdate_GUI()
{
	DataUnion attr;
	
	_WriteAttribute( ATTR_MAX_PARTICLES,		attr.SetInt( _target->GetMaxParticles() ) );
	_WriteAttribute( ATTR_PARTICLE_LIFETIME,	attr.SetFloat( _target->GetParticleLifetime() ) );
	_WriteAttribute( ATTR_PARTICLE_VELOCITY,	attr.SetVector3( _target->GetParticleVelocity() ) );
	_WriteAttribute( ATTR_PARTICLE_ACCEL,		attr.SetVector3( _target->GetParticleAccel() ) );
	_WriteAttribute( ATTR_SHAPE_DRIVEN_SPEED,	attr.SetFloat( _target->GetShapeDrivenSpeed() ) );
	_WriteAttribute( ATTR_SHAPE_DRIVEN_ACCEL,	attr.SetFloat( _target->GetShapeDrivenAccel() ) );

	_WriteAttribute( ATTR_WAVES_PER_SEC,		attr.SetFloat( _target->GetWavesPerSec() ) );
	_WriteAttribute( ATTR_PARTICLES_PER_WAVE,	attr.SetInt( _target->GetParticlesPerWave() ) );
	_WriteAttribute( ATTR_IS_LOOPING,			attr.SetBool( _target->IsLooping() ) );
	_WriteAttribute( ATTR_SPAWN_DURATION,		attr.SetFloat( _target->GetSpawnDuration() ) );

	_WriteAttribute( ATTR_EMITTER_SHAPE,		attr.SetInt( _GetIndexForType( _target->GetEmitterShape() ) ) );
	_WriteAttribute( ATTR_SHAPE_RADIUS,			attr.SetFloat( _target->GetShapeRadius() ) );
	_WriteAttribute( ATTR_SPAWN_FROM_EDGE,		attr.SetBool( _target->DoesSpawnFromEdge() ) );
	
	_WriteAttribute( ATTR_SIZE_START,			attr.SetVector2( _target->GetParticleSizeStart() ) );
	_WriteAttribute( ATTR_SIZE_END,				attr.SetVector2( _target->GetParticleSizeEnd() ) );

	Material* material = _target->GetMaterial();
	const Char* matName = (material == NULL) ? Selector_Material::OPTION_NULL : material->GetName();
	_WriteAttribute( ATTR_MATERIAL,				attr.SetReference( material ), matName );


	_WriteInnerAttribute( InnerAttrType::ENABLED,	attr.SetBool( _target->IsEnabled() ) );

	_Colorize( _target->IsEnabled() );
}


void AttrViewer_ParticleEmitter::_OnUpdate_Target()
{
	_target->SetEmitEvents( FALSE );

	_target->SetMaxParticles		( _ReadAttribute( ATTR_MAX_PARTICLES ).GetInt() );
	_target->SetParticleLifetime	( _ReadAttribute( ATTR_PARTICLE_LIFETIME ).GetFloat() );
	_target->SetParticleVelocity	( _ReadAttribute( ATTR_PARTICLE_VELOCITY ).GetVector3() );
	_target->SetParticleAccel		( _ReadAttribute( ATTR_PARTICLE_ACCEL ).GetVector3() );
	_target->SetShapeDrivenSpeed	( _ReadAttribute( ATTR_SHAPE_DRIVEN_SPEED ).GetFloat() );
	_target->SetShapeDrivenAccel	( _ReadAttribute( ATTR_SHAPE_DRIVEN_ACCEL ).GetFloat() );

	_target->SetWavesPerSec			( _ReadAttribute( ATTR_WAVES_PER_SEC ).GetFloat() );
	_target->SetParticlesPerWave	( _ReadAttribute( ATTR_PARTICLES_PER_WAVE ).GetInt() );
	_target->SetIsLooping			( _ReadAttribute( ATTR_IS_LOOPING ).GetBool() );
	_target->SetSpawnDuration		( _ReadAttribute( ATTR_SPAWN_DURATION ).GetFloat() );

	_target->SetEmitterShape		( _GetTypeForIndex( _ReadAttribute( ATTR_EMITTER_SHAPE ).GetInt() ) );
	_target->SetShapeRadius			( _ReadAttribute( ATTR_SHAPE_RADIUS ).GetFloat() );
	_target->SetSpawnFromEndge		( _ReadAttribute( ATTR_SPAWN_FROM_EDGE ).GetBool() );
			
	_target->SetParticleSizeStart	( _ReadAttribute( ATTR_SIZE_START ).GetVector2() );
	_target->SetParticleSizeEnd		( _ReadAttribute( ATTR_SIZE_END ).GetVector2() );
	_target->SetMaterial			( CAST_S( Material*, _ReadAttribute( ATTR_MATERIAL ).GetReference() ) );

	_target->SetEnabled( _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() );

	_target->BindToGPU();

	_target->SetEmitEvents( TRUE );

	// emit event manually
	EventManager::Singleton()->EmitEvent( EventType::COMPONENT, 
										  EventName::PARTICLE_EMITTER_CHANGED, 
										  _target );

	_Colorize( _target->IsEnabled() );
}


Int AttrViewer_ParticleEmitter::_GetIndexForType( EmitterShape type )
{
	switch ( type )
	{
		case EmitterShape::CIRCLE:
			return 0;

		case EmitterShape::SPHERE:
			return 1;
	}

	return -1;
}


EmitterShape AttrViewer_ParticleEmitter::_GetTypeForIndex( Int index )
{
	switch ( index )
	{
		case 0:
			return EmitterShape::CIRCLE;

		case 1:
			return EmitterShape::SPHERE;
	}

	return EmitterShape::CIRCLE;
}

