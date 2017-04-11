// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../1_Required/Required.h"

#include "String\String.h"
#include "Data\DataArray.h"

#include "Composite\Node.h"


namespace CYRED
{
	class Component;
}


namespace CYRED
{
	//!		only SceneManager should create a new GameObject and give the UID
	class DLL GameObject : public Node
	{
	public:
		//! created without UID, for internal use only
		GameObject();

		GameObject( UInt uid );
		GameObject( const Char* name, UInt uid );
		virtual ~GameObject();


	public:
		void OnHierarchyChange	()					override;
		void OnUpdate			( Bool isRuntime )	override;


	public:
		const Char*	GetName				()						const;
		Bool		IsEnabled			()						const;
		UInt		GetUniqueID			()						const;
		Bool		DoesEmitEvents		()						const;

		UInt		GetComponentCount	()						const;
		Component*	GetComponentAt		( UInt index )			const;

		void		SetEnabled			( Bool value );
		void		SetName				( const Char* name );
		void		SetEmitEvents		( Bool value );

		void		Clone				( GameObject* clone )	const;


	public:
		template <class T> T*	GetComponent	()	const;
		template <class T> T*	AddComponent	();
		template <class T> void	RemoveComponent	();


	protected:
		String	_name;
		Bool	_enabled;
		UInt	_uid;	// unique index
		Bool	_emitEvents;

		DataArray<Component*>	_components;

		void _SendChangeEvent();
	};



	template <class T>
	T* GameObject::GetComponent() const
	{
		STATIC_ASSERT( IS_BASE_OF( Component, T ), "Class must derive Component" );

		T*	requested;

		for ( UInt i = 0; i < _components.Size(); ++i ) {
			requested = dynamic_cast<T*>( _components[i] );

			if ( requested != NULL ) {
				return requested;
			}
		}

		return NULL;
	}


	template <class T>
	T* GameObject::AddComponent()
	{
		STATIC_ASSERT( IS_BASE_OF( Component, T ), "Class must derive Component" );

		T*	onlyOneAllowed;

		for ( UInt i = 0; i < _components.Size(); ++i ) {
			onlyOneAllowed = dynamic_cast<T*>( _components[i] );

			if ( onlyOneAllowed != NULL ) {
				return onlyOneAllowed;
			}
		}

		// not found so far
		onlyOneAllowed =  Memory::Alloc<T>( this );
		_components.Add( onlyOneAllowed );

		Component* comp = CAST_S( Component*, onlyOneAllowed );
		comp->SetEnabled( TRUE );

		// emit change event
		_SendChangeEvent();

		return onlyOneAllowed;
	}


	template <class T>
	void GameObject::RemoveComponent()
	{
		STATIC_ASSERT( IS_BASE_OF( Component, T ), "Class must derive Component" );

		T*	onlyOneAllowed;

		for ( UInt i = 0; i < _components.Size(); ++i ) {
			onlyOneAllowed = dynamic_cast<T*>( _components[i] );

			if ( onlyOneAllowed != NULL ) {
				_components.Erase( i );
				Memory::Free( onlyOneAllowed );

				// emit change event
				_SendChangeEvent();

				return;
			}
		}
	}
}