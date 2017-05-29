// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "../Data/DataArray.h"
#include "../String/String.h"


namespace CYRED
{
	//! class for creating parent-child hierarchy
	class DLL Node
	{
	public:
		Node();
		virtual ~Node();


	public:
		//! callback for when the parent/ children links change
		virtual void OnHierarchyChange	() {};
		virtual void OnUpdate			( bool isRuntime ) {};


	public:
		Node*	GetParentNode		()							const;
		void	SetParentNode		( Node* newNode );
		void	AddChildNode		( Node* newNode );
		void	InsertChildNode		( int index, Node* newNode );

		int		GetChildNodeCount	()							const;
		Node*	GetChildNodeAt		( int index )				const;

		void	DeleteAllChildNodes	();


	protected:
		Node*				_parentNode;	//! the parent
		DataArray<Node*>	_childNodes;	//! list of children


	protected:
		void _RemoveChildNode( Node* childNode );
	};

}