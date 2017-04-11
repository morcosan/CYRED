// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "GameObject.h"
#include "Component.h"
#include "../3_Modules/Event/EventManager.h"
#include "Components\Transform.h"
#include "../3_Modules/Render/Components/Camera.h"
#include "../3_Modules/Render/Components/MeshRendering.h"
#include "../3_Modules/Render/Components/MorphRendering.h"
#include "../3_Modules/Render/Components/ParticleEmitter.h"
#include "../3_Modules/Script/Components/Scripter.h"


using namespace CYRED;



GameObject::GameObject()
	: _uid( EMPTY_OBJECT_UID )
	, _enabled ( TRUE )
	, _emitEvents( TRUE )
{
}


GameObject::GameObject( UInt uid )
	: _uid( uid )
	, _enabled ( TRUE )
	, _emitEvents( TRUE )
{
}

GameObject::GameObject( const Char* name, UInt uid )
	: _name( name )
	, _uid( uid )
	, _enabled ( TRUE )
	, _emitEvents( TRUE )
{
}


GameObject::~GameObject()
{
	for ( UInt i = 0; i < _components.Size(); ++i )
	{
		Memory::Free( _components[i] );
	}

	_components.Clear();
}


void GameObject::OnHierarchyChange()
{
	for ( UInt i = 0; i < _components.Size(); ++i )
	{
		_components[i]->OnHierarchyChange();
	}
}


void GameObject::OnUpdate( Bool isRuntime )
{
	// update components
	for ( UInt i = 0; i < _components.Size(); ++i )	{
		_components[i]->OnUpdate( isRuntime );
	}

	// update child gameobjects
	for ( UInt i = 0; i < _childNodes.Size(); ++i )	{
		_childNodes[i]->OnUpdate( isRuntime );
	}
}


Bool GameObject::IsEnabled() const
{
	return _enabled;
}


UInt GameObject::GetUniqueID() const
{
	return _uid;
}


Bool GameObject::DoesEmitEvents() const
{
	return _emitEvents;
}


UInt GameObject::GetComponentCount() const
{
	return _components.Size();
}


Component* GameObject::GetComponentAt( UInt index ) const
{
	ASSERT( index < _components.Size() );
	return _components[index];
}


void GameObject::SetEnabled( Bool value )
{
	_enabled = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_GAMEOBJECT, this );
	}
}


const Char* GameObject::GetName() const
{
	return _name.GetChar();
}


void GameObject::SetName( const Char* name )
{
	_name = name;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::RENAME_GAMEOBJECT, this );
	}
}


void GameObject::SetEmitEvents( Bool value )
{
	_emitEvents = value;
}


void GameObject::Clone( GameObject* clone ) const
{
	// create clone of this gameobject
	clone->SetEnabled( _enabled );
	clone->SetName( _name.GetChar() );

	for ( UInt i = 0; i < _components.Size(); i++ ) {
		Component* cloneComp = NULL;

		switch ( _components[i]->GetComponentType() ) {
			case ComponentType::CAMERA:
				cloneComp = clone->AddComponent<Camera>();
				break;
			case ComponentType::LIGHT:
				break;

			case ComponentType::MESH_RENDERING:
				cloneComp = clone->AddComponent<MeshRendering>();
				break;

			case ComponentType::MORPH_RENDERING:
				cloneComp = clone->AddComponent<MorphRendering>();
				break;

			case ComponentType::PARTICLE_EMITTER:
				cloneComp = clone->AddComponent<ParticleEmitter>();
				break;

			case ComponentType::TRANSFORM:
				cloneComp = clone->AddComponent<Transform>();
				break;

			case ComponentType::SCRIPTER:
				cloneComp = clone->AddComponent<Scripter>();
				break;
		}

		// clone component
		cloneComp->SetEmitEvents( FALSE );
		_components[i]->Clone( cloneComp );
		cloneComp->SetEmitEvents( TRUE );
	}
}


void GameObject::_SendChangeEvent()
{
	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_GAMEOBJECT, this );
	}
}

