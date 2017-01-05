// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"

#include "../../../2_BuildingBlocks/Component.h"


namespace CYRED
{
	class Morph;
	class Material;
}


namespace CYRED
{
	class DLL MorphRendering : public Component
	{
	public:
		MorphRendering( GameObject* gameObject = NULL );
		virtual ~MorphRendering() {}


	public:
		void Clone( Component* clone ) const override;


	public:
		Material*	GetMaterial				() const;
		Morph*		GetMorph				() const;
		Float		GetDurationState		() const;
		Float		GetDurationStateChange	() const;
		Bool		IsPlaying				() const;

		UInt		GetFirstState			() const;
		UInt		GetSecondState			() const;
		Float		GetStateRatio			() const;

		void		SetMaterial				( Material* value );
		void		SetMorph				( Morph* value );
		void		SetDurationState		( Float value );
		void		SetDurationStateChange	( Float value );
		void		SetIsPlaying			( Bool value );


	protected:
		void _OnEnable()				 override {}
		void _OnUpdate( Bool isRuntime ) override;


	protected:
		Material*	_material;
		Morph*		_morph;

		Float		_durationState;
		Float		_durationStateChange;
		Bool		_isPlaying;

		UInt		_firstState;
		UInt		_secondState;
		Float		_currDuration;
		Float		_currTime;
		Bool		_isInState;
	};
}