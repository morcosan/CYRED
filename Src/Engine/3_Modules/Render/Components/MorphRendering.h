// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
		float		GetDurationState		() const;
		float		GetDurationStateChange	() const;
		bool		IsPlaying				() const;

		int		GetFirstState			() const;
		int		GetSecondState			() const;
		float		GetStateRatio			() const;

		void		SetMaterial				( Material* value );
		void		SetMorph				( Morph* value );
		void		SetDurationState		( float value );
		void		SetDurationStateChange	( float value );
		void		SetIsPlaying			( bool value );


	protected:
		void _OnEnable()				 override {}
		void _OnUpdate( bool isRuntime ) override;


	protected:
		Material*	_material;
		Morph*		_morph;

		float		_durationState;
		float		_durationStateChange;
		bool		_isPlaying;

		int		_firstState;
		int		_secondState;
		float		_currDuration;
		float		_currTime;
		bool		_isInState;
	};
}