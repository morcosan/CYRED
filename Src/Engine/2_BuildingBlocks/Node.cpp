// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Node.h"


using namespace CYRED;


Node::Node()
	: _parentNode( NULL )
{
}


Node::Node( cchar* name ) 
	: _name( name )
	, _parentNode( NULL )
{
}


Node::~Node()
{
	// delete child nodes
	DeleteAllChildNodes();

	// should have no children by now
	if ( _parentNode != NULL ) {
		_parentNode->_RemoveChildNode( this );
	}
}


Node* Node::GetParentNode() const
{
	return _parentNode;
}


void Node::SetParentNode( Node* newNode )
{
	// not to waste resources
	if ( _parentNode != newNode ) {
		Node* lastParent = _parentNode;
		_parentNode = newNode;

		if ( newNode != NULL ) {
			newNode->AddChildNode( this );
		}

		if ( lastParent != NULL ) {
			lastParent->_RemoveChildNode( this );
		}

		OnHierarchyChange();	//! callback
	}
}


void Node::AddChildNode( Node* newNode )
{
	//! sanity check
	if ( newNode == NULL ) {
		return;		
	}

	for ( int i = 0; i < _childNodes.Size(); ++i ) {
		if ( _childNodes[i] == newNode ) {
			//! cannot add same child more times
			return;
		}
	}

	_childNodes.Add( newNode );
	newNode->SetParentNode( this );

	OnHierarchyChange();	//! callback
}


void Node::InsertChildNode( int index, Node* newNode )
{
	//! sanity check
	if ( newNode == NULL ) {
		return;		
	}

	for ( int i = 0; i < _childNodes.Size(); ++i ) {
		if ( _childNodes[i] == newNode ) {
			//! cannot add same child more times
			return;
		}
	}

	_childNodes.Insert( index, newNode );
	newNode->SetParentNode( this );

	OnHierarchyChange();	//! callback
}


int Node::GetChildNodeCount() const
{
	return _childNodes.Size();
}


Node* Node::GetChildNodeAt( int index ) const
{
	ASSERT( index < _childNodes.Size() );

	return _childNodes[index];
}


void Node::DeleteAllChildNodes()
{
	// the child will remove itself from parent
	while ( _childNodes.Size() > 0 ) {
		Memory::Free( _childNodes[0] );
	}
}


void Node::_RemoveChildNode( Node* childNode )
{
	ASSERT( childNode != NULL );

	for ( int i = 0; i < _childNodes.Size(); ++i )	{
		if ( _childNodes[i] == childNode ) {
			_childNodes.Erase( i );
			return;
		}
	}
}


cchar* Node::GetName() const
{
	return _name.GetChar();
}


void Node::SetName( cchar* name )
{
	_name = name;
	_OnRename();
}
