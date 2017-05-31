// Copyright (c) 2015-2017 Morco (www.morco.ro)
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


void AttrViewer_ParticleEmitter::_OnInitialize()
{
	_OpenGroup( GROUP_PARTICLES );
	_CreateAttrInt		( ATTR_MAX_PARTICLES,		ATTR_MAX_PARTICLES );
	_CreateAttrFloat	( ATTR_PARTICLE_LIFETIME,	ATTR_PARTICLE_LIFETIME );
	_CreateAttrVector3	( ATTR_PARTICLE_VELOCITY,	ATTR_PARTICLE_VELOCITY );
	_CreateAttrVector3	( ATTR_PARTICLE_ACCEL,		ATTR_PARTICLE_ACCEL );
	_CreateAttrFloat	( ATTR_SHAPE_DRIVEN_SPEED,	ATTR_SHAPE_DRIVEN_SPEED );
	_CreateAttrFloat	( ATTR_SHAPE_DRIVEN_ACCEL,	ATTR_SHAPE_DRIVEN_ACCEL );
	_CloseGroup();

	_OpenGroup( GROUP_SPAWNING );
	_CreateAttrFloat	( ATTR_WAVES_PER_SEC,		ATTR_WAVES_PER_SEC );
	_CreateAttrInt		( ATTR_PARTICLES_PER_WAVE,	ATTR_PARTICLES_PER_WAVE );
	_CreateAttrBool		( ATTR_IS_LOOPING,			ATTR_IS_LOOPING );
	_CreateAttrFloat	( ATTR_SPAWN_DURATION,		ATTR_SPAWN_DURATION );
	_CloseGroup();

	_OpenGroup( GROUP_EMISSION );
	_CreateAttrDropdown	( ATTR_EMITTER_SHAPE,	ATTR_EMITTER_SHAPE, _emitterShapes );
	_CreateAttrFloat	( ATTR_SHAPE_RADIUS,	ATTR_SHAPE_RADIUS );
	_CreateAttrBool		( ATTR_SPAWN_FROM_EDGE,	ATTR_SPAWN_FROM_EDGE );
	_CloseGroup();

	_OpenGroup( GROUP_RENDERING );
	_CreateAttrVector2	( ATTR_SIZE_START,	ATTR_SIZE_START );
	_CreateAttrVector2	( ATTR_SIZE_END,	ATTR_SIZE_END );
	_CreateAttrSelector	( ATTR_MATERIAL,	ATTR_MATERIAL, Selector_Material::TYPE );
	_CloseGroup();
	
	_CreateInnerAttribute( InnerAttrType::ENABLED );
	_CreateInnerAttribute( InnerAttrType::SETTINGS );

	_AddToPanel( TITLE );
}


void AttrViewer_ParticleEmitter::_OnChangeTarget( void* target )
{
	_target = CAST_S( ParticleEmitter*, target );

	// prepare settings
	DataUnion attr;
	_WriteInnerAttribute( InnerAttrType::SETTINGS, attr.SetReference( _target ) );
}


void AttrViewer_ParticleEmitter::_UpdateGUI()
{
	_WriteAttrInt		( ATTR_MAX_PARTICLES,		_target->GetMaxParticles() );
	_WriteAttrFloat		( ATTR_PARTICLE_LIFETIME,	_target->GetParticleLifetime() );
	_WriteAttrVector3	( ATTR_PARTICLE_VELOCITY,	_target->GetParticleVelocity() );
	_WriteAttrVector3	( ATTR_PARTICLE_ACCEL,		_target->GetParticleAccel() );
	_WriteAttrFloat		( ATTR_SHAPE_DRIVEN_SPEED,	_target->GetShapeDrivenSpeed() );
	_WriteAttrFloat		( ATTR_SHAPE_DRIVEN_ACCEL,	_target->GetShapeDrivenAccel() );

	_WriteAttrFloat		( ATTR_WAVES_PER_SEC,		_target->GetWavesPerSec() );
	_WriteAttrInt		( ATTR_PARTICLES_PER_WAVE,	_target->GetParticlesPerWave() );
	_WriteAttrBool		( ATTR_IS_LOOPING,			_target->IsLooping() );
	_WriteAttrFloat		( ATTR_SPAWN_DURATION,		_target->GetSpawnDuration() );

	_WriteAttrDropdown	( ATTR_EMITTER_SHAPE,		_GetIndexForType( _target->GetEmitterShape() ) );
	_WriteAttrFloat		( ATTR_SHAPE_RADIUS,		_target->GetShapeRadius() );
	_WriteAttrBool		( ATTR_SPAWN_FROM_EDGE,		_target->DoesSpawnFromEdge() );
	
	_WriteAttrVector2	( ATTR_SIZE_START,			_target->GetParticleSizeStart() );
	_WriteAttrVector2	( ATTR_SIZE_END,			_target->GetParticleSizeEnd() );

	Material* material = _target->GetMaterial();
	cchar* matName = (material == NULL) ? Selector_Material::OPTION_NULL : material->GetName();
	_WriteAttrSelector	( ATTR_MATERIAL,			material, matName );

	if ( _target->IsEnabled() != _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() )
	{
		DataUnion attr;
		_WriteInnerAttribute( InnerAttrType::ENABLED, attr.SetBool( _target->IsEnabled() ) );

		_Colorize( _target->IsEnabled() );
	}
}


void AttrViewer_ParticleEmitter::_UpdateTarget()
{
	_target->SetEmitEvents( FALSE );

	_target->SetMaxParticles		( _ReadAttrInt( ATTR_MAX_PARTICLES ) );
	_target->SetParticleLifetime	( _ReadAttrFloat( ATTR_PARTICLE_LIFETIME ) );
	_target->SetParticleVelocity	( _ReadAttrVector3( ATTR_PARTICLE_VELOCITY ) );
	_target->SetParticleAccel		( _ReadAttrVector3( ATTR_PARTICLE_ACCEL ) );
	_target->SetShapeDrivenSpeed	( _ReadAttrFloat( ATTR_SHAPE_DRIVEN_SPEED ) );
	_target->SetShapeDrivenAccel	( _ReadAttrFloat( ATTR_SHAPE_DRIVEN_ACCEL ) );

	_target->SetWavesPerSec			( _ReadAttrFloat( ATTR_WAVES_PER_SEC ) );
	_target->SetParticlesPerWave	( _ReadAttrInt( ATTR_PARTICLES_PER_WAVE ) );
	_target->SetIsLooping			( _ReadAttrBool( ATTR_IS_LOOPING ) );
	_target->SetSpawnDuration		( _ReadAttrFloat( ATTR_SPAWN_DURATION ) );

	_target->SetEmitterShape		( _GetTypeForIndex( _ReadAttrDropdown( ATTR_EMITTER_SHAPE ) ) );
	_target->SetShapeRadius			( _ReadAttrFloat( ATTR_SHAPE_RADIUS ) );
	_target->SetSpawnFromEndge		( _ReadAttrBool( ATTR_SPAWN_FROM_EDGE ) );
			
	_target->SetParticleSizeStart	( _ReadAttrVector2( ATTR_SIZE_START ) );
	_target->SetParticleSizeEnd		( _ReadAttrVector2( ATTR_SIZE_END ) );
	_target->SetMaterial			( CAST_S( Material*, _ReadAttrSelector( ATTR_MATERIAL ) ) );

	_target->SetEnabled( _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() );

	_target->BindToGPU();

	_target->SetEmitEvents( TRUE );

	// emit event manually
	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, _target );

	_Colorize( _target->IsEnabled() );
}


int AttrViewer_ParticleEmitter::_GetIndexForType( EmitterShape type )
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


EmitterShape AttrViewer_ParticleEmitter::_GetTypeForIndex( int index )
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

