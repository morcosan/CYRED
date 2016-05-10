// Copyright (c) 2015 Morco (www.morco.ro)
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
		virtual void OnUpdate			() {};


	public:
		Node*	GetParentNode	()					const;
		void	SetParentNode	( Node* newNode );
		void	AddChildNode	( Node* newNode );

		UInt	GetChildNodeCount	()				const;
		Node*	GetChildNodeAt		( UInt index );


	protected:
		Node*				_parentNode;	//! the parent
		DataArray<Node*>	_childNodes;	//! list of children

		void _RemoveChildNode( Node* childNode );
	};

}