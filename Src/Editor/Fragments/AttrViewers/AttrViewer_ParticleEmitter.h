// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"
#include "CyredModule_Render.h"


namespace CYRED
{
	namespace COMP
	{
		class ParticleEmitter;
	}
}


namespace CYRED
{
	class AttrViewer_ParticleEmitter : public AttrViewer
	{
	public:
		const Char* TITLE					= "ParticleEmitter";

		const Char* ATTR_MAX_PARTICLES		= "Max Particles";
		const Char* ATTR_PARTICLE_LIFETIME	= "Particle Lifetime";
		const Char* ATTR_PARTICLE_VELOCITY	= "Particle Velocity";
		const Char* ATTR_PARTICLE_ACCEL		= "Particle Acceleration";
		const Char* ATTR_SHAPE_DRIVEN_SPEED	= "Shape-Driven Speed";
		const Char* ATTR_SHAPE_DRIVEN_ACCEL	= "Shape-Driven Accel";

		const Char* ATTR_WAVES_PER_SEC		= "Waves / Sec";
		const Char* ATTR_PARTICLES_PER_WAVE	= "Particles / Wave";
		const Char* ATTR_IS_LOOPING			= "Looping";
		const Char* ATTR_SPAWN_DURATION		= "Spawn Duration";

		const Char* ATTR_EMITTER_SHAPE		= "Emitter Shape";
		const Char* ATTR_SHAPE_RADIUS		= "Shape Radius";
		const Char* ATTR_SPAWN_FROM_EDGE	= "Spawn From Edge";

		const Char* ATTR_SIZE_START			= "P.Size Start";
		const Char* ATTR_SIZE_END			= "P.Size End";
		const Char* ATTR_MATERIAL			= "Material";

		const Char* GROUP_PARTICLES			= "Simulation";
		const Char* GROUP_SPAWNING			= "Spawning";
		const Char* GROUP_EMISSION			= "Emission";
		const Char* GROUP_RENDERING			= "Rendering";


	public:
		AttrViewer_ParticleEmitter();
		virtual ~AttrViewer_ParticleEmitter() {}


	public:
		void OnSelect_Target	( void* target )	override;
	

	protected:
		void _OnUpdate_GUI		()	override;
		void _OnUpdate_Target	()	override;


	protected:
		COMP::ParticleEmitter*	_target;

		DataArray<const Char*>	_emitterShapes;

		Int			_GetIndexForType( EmitterShape type );
		EmitterShape	_GetTypeForIndex( Int index );
	};
}