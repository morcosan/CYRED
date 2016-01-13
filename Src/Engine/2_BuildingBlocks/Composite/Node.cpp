// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Node.h"


using namespace CYRED;


Node::Node()
	: _parentNode( NULL )
{
}


Node::~Node()
{
	// the child will remove itself from parent
	while ( _childNodes.Size() > 0 )
	{
		Memory::Free( _childNodes[0] );
	}

	// should have no children by now
	if ( _parentNode != NULL )
	{
		_parentNode->_RemoveChildNode( this );
	}
}


Node* Node::GetParentNode() const
{
	return _parentNode;
}


void Node::SetParentNode( Node* newNode )
{
	if ( _parentNode != newNode )	//! not to waste resources
	{
		Node* lastParent = _parentNode;
		_parentNode = newNode;

		if ( newNode != NULL )
		{
			newNode->AddChildNode( this );
		}

		if ( lastParent != NULL )
		{
			lastParent->_RemoveChildNode( this );
		}

		OnHierarchyChange();	//! callback
	}
}


void Node::AddChildNode( Node* newNode )
{
	if ( newNode == NULL )		//! sanity check
	{
		return;		
	}

	for ( UInt i = 0; i < _childNodes.Size(); ++i )
	{
		if ( _childNodes[i] == newNode )	//! cannot add same child more times
		{
			return;
		}
	}

	_childNodes.Add( newNode );
	newNode->SetParentNode( this );

	OnHierarchyChange();	//! callback
}


UInt Node::GetChildNodeCount() const
{
	return _childNodes.Size();
}


Node* Node::GetChildNodeAt( UInt index )
{
	ASSERT( index < _childNodes.Size() );

	return _childNodes[index];
}


void Node::_RemoveChildNode( Node* childNode )
{
	ASSERT( childNode != NULL );

	for ( UInt i = 0; i < _childNodes.Size(); ++i )
	{
		if ( _childNodes[i] == childNode )
		{
			_childNodes.Erase( i );
			return;
		}
	}
}
