// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
		const char* TITLE					= "ParticleEmitter";

		const char* ATTR_MAX_PARTICLES		= "Max Particles";
		const char* ATTR_PARTICLE_LIFETIME	= "Particle Lifetime";
		const char* ATTR_PARTICLE_VELOCITY	= "Particle Velocity";
		const char* ATTR_PARTICLE_ACCEL		= "Particle Acceleration";
		const char* ATTR_SHAPE_DRIVEN_SPEED	= "Shape-Driven Speed";
		const char* ATTR_SHAPE_DRIVEN_ACCEL	= "Shape-Driven Accel";

		const char* ATTR_WAVES_PER_SEC		= "Waves / Sec";
		const char* ATTR_PARTICLES_PER_WAVE	= "Particles / Wave";
		const char* ATTR_IS_LOOPING			= "Looping";
		const char* ATTR_SPAWN_DURATION		= "Spawn Duration";

		const char* ATTR_EMITTER_SHAPE		= "Emitter Shape";
		const char* ATTR_SHAPE_RADIUS		= "Shape Radius";
		const char* ATTR_SPAWN_FROM_EDGE	= "Spawn From Edge";

		const char* ATTR_SIZE_START			= "P.Size Start";
		const char* ATTR_SIZE_END			= "P.Size End";
		const char* ATTR_MATERIAL			= "Material";

		const char* GROUP_PARTICLES			= "Simulation";
		const char* GROUP_SPAWNING			= "Spawning";
		const char* GROUP_EMISSION			= "Emission";
		const char* GROUP_RENDERING			= "Rendering";


	public:
		AttrViewer_ParticleEmitter();
		virtual ~AttrViewer_ParticleEmitter() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		ParticleEmitter*	_target;

		DataArray<const char*>	_emitterShapes;

		int				_GetIndexForType( EmitterShape type );
		EmitterShape	_GetTypeForIndex( int index );
	};
}