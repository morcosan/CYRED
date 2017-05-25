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
		cchar* TITLE					= "ParticleEmitter";

		cchar* ATTR_MAX_PARTICLES		= "Max Particles";
		cchar* ATTR_PARTICLE_LIFETIME	= "Particle Lifetime";
		cchar* ATTR_PARTICLE_VELOCITY	= "Particle Velocity";
		cchar* ATTR_PARTICLE_ACCEL		= "Particle Acceleration";
		cchar* ATTR_SHAPE_DRIVEN_SPEED	= "Shape-Driven Speed";
		cchar* ATTR_SHAPE_DRIVEN_ACCEL	= "Shape-Driven Accel";

		cchar* ATTR_WAVES_PER_SEC		= "Waves / Sec";
		cchar* ATTR_PARTICLES_PER_WAVE	= "Particles / Wave";
		cchar* ATTR_IS_LOOPING			= "Looping";
		cchar* ATTR_SPAWN_DURATION		= "Spawn Duration";

		cchar* ATTR_EMITTER_SHAPE		= "Emitter Shape";
		cchar* ATTR_SHAPE_RADIUS		= "Shape Radius";
		cchar* ATTR_SPAWN_FROM_EDGE	= "Spawn From Edge";

		cchar* ATTR_SIZE_START			= "P.Size Start";
		cchar* ATTR_SIZE_END			= "P.Size End";
		cchar* ATTR_MATERIAL			= "Material";

		cchar* GROUP_PARTICLES			= "Simulation";
		cchar* GROUP_SPAWNING			= "Spawning";
		cchar* GROUP_EMISSION			= "Emission";
		cchar* GROUP_RENDERING			= "Rendering";


	public:
		AttrViewer_ParticleEmitter();
		virtual ~AttrViewer_ParticleEmitter() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	private:
		ParticleEmitter*	_target;

		DataArray<cchar*>	_emitterShapes;

		int				_GetIndexForType( EmitterShape type );
		EmitterShape	_GetTypeForIndex( int index );
	};
}