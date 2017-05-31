// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../1_Required/Required.h"

#include "Node.h"

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

		GameObject( int uid );
		GameObject( cchar* name, int uid );
		virtual ~GameObject();


	public:
		void OnHierarchyChange	()					override;
		void OnUpdate			( bool isRuntime )	override;


	public:
		bool		IsEnabled			()						const;
		int			GetUniqueID			()						const;
		bool		DoesEmitEvents		()						const;

		int			GetComponentCount	()						const;
		Component*	GetComponentAt		( int index )			const;

		void		SetEnabled			( bool value );
		void		SetEmitEvents		( bool value );

		void		Clone				( GameObject* clone )	const;


	public:
		template <class T> T*	GetComponent	()	const;
		template <class T> T*	AddComponent	();
		template <class T> void	RemoveComponent	();


	protected:
		void _OnRename() override;


	private:
		bool	_enabled;
		int		_uid;	// unique index
		bool	_emitEvents;

		DataArray<Component*>	_components;


	private:
		void _EmitAddEvent		();
		void _EmitRemoveEvent	();
	};



	template <class T>
	T* GameObject::GetComponent() const
	{
		STATIC_ASSERT( IS_BASE_OF( Component, T ), "Class must derive Component" );

		T*	requested;

		for ( int i = 0; i < _components.Size(); ++i ) {
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

		for ( int i = 0; i < _components.Size(); ++i ) {
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

		// emit event
		_EmitAddEvent();

		return onlyOneAllowed;
	}


	template <class T>
	void GameObject::RemoveComponent()
	{
		STATIC_ASSERT( IS_BASE_OF( Component, T ), "Class must derive Component" );

		T*	onlyOneAllowed;

		for ( int i = 0; i < _components.Size(); ++i ) {
			onlyOneAllowed = dynamic_cast<T*>( _components[i] );

			if ( onlyOneAllowed != NULL ) {
				_components.Erase( i );
				Memory::Free( onlyOneAllowed );

				// emit event
				_EmitRemoveEvent();

				return;
			}
		}
	}
}