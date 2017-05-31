// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "MorphRendering.h"

#include "../Assets/Material.h"
#include "../Assets/Morph.h"

#include "../../Time/TimeManager.h"
#include "../../Event/EventManager.h"


using namespace CYRED;


MorphRendering::MorphRendering( GameObject* gameObject )
	: Component( gameObject )
	, _material( NULL )
	, _morph( NULL )
	, _durationState( 1.0f )
	, _durationStateChange( 1.0f )
	, _isPlaying( TRUE )
	, _firstState( 0 )
	, _secondState( 0 )
	, _currTime( 0 )
	, _currDuration( 0 )
	, _isInState( TRUE )
{
	_componentType = ComponentType::MORPH_RENDERING;
}



void MorphRendering::Clone( Component* clone ) const
{
}

Material* MorphRendering::GetMaterial() const
{
	return _material;
}


Morph* MorphRendering::GetMorph() const
{
	return _morph;
}


float MorphRendering::GetDurationState() const
{
	return _durationState;
}


float MorphRendering::GetDurationStateChange() const
{
	return _durationStateChange;
}


bool MorphRendering::IsPlaying() const
{
	return _isPlaying;
}


int MorphRendering::GetFirstState() const
{
	return _firstState;
}


int MorphRendering::GetSecondState() const
{
	return _secondState;
}


float MorphRendering::GetStateRatio() const
{
	return (_currDuration > 0) ? (_currTime / _currDuration) : 0.0f;
}


void MorphRendering::SetMaterial( Material* value )
{
	_material = value;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, this );
	}
}


void MorphRendering::SetMorph( Morph* value )
{
	_morph = value;

	_isInState = TRUE;
	_currTime = 0;
	_currDuration = _durationState;

	_firstState = 0;
	_secondState = 0;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, this );
	}
}


void MorphRendering::SetDurationState( float value )
{
	_durationState = (value > 0.0f) ? value : 0.0f;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, this );
	}
}


void MorphRendering::SetDurationStateChange( float value )
{
	_durationStateChange = (value > 0.0f) ? value : 0.0f;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, this );
	}
}


void MorphRendering::SetIsPlaying( bool value )
{
	_isPlaying = value;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, this );
	}
}


void MorphRendering::_OnUpdate( bool isRuntime )
{
	if ( _morph != NULL && _isPlaying )
	{
		_currTime += TimeManager::Singleton()->GetDeltaTime();

		if ( _currTime >= _currDuration ) // change state
		{
			_isInState = !_isInState;
			_currTime = 0.0f;
			_currDuration = _isInState ? _durationState : _durationStateChange;

			int numStates = _morph->GetActiveStates();

			if ( !_isInState )
			{
				if ( numStates > 1 )
				{
					_secondState = (_firstState + 1) % numStates;
				}
			}
			else
			{
				_firstState = _secondState;
			}
		}
	}
}

