// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "CustomTree.h"

#include "CyredBuildingBlocks.h"
#include "CyredModule_Event.h"
#include "CyredModule_Asset.h"

#include "CustomTreeItem.h"

#include "QtGui\qevent.h"


using namespace CYRED;


void CustomTree::mousePressEvent( QMouseEvent* event )
{
	QModelIndex item = indexAt( event->pos() );
	bool selected = selectionModel()->isSelected( indexAt( event->pos() ) );
	QTreeView::mousePressEvent( event );
	if ( (item.row() == -1 && item.column() == -1) || selected ) {
		this->clearSelection();
		// send event
		EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, NULL );
	}
}


void CustomTree::dropEvent( QDropEvent* event )
{
	// get moved item
	CustomTreeItem* movedItem = CAST_S( CustomTreeItem*, this->currentItem() );
	// get old parent item
	CustomTreeItem* prevParent = CAST_S( CustomTreeItem*, movedItem->parent() );
	// get the order in the old hierarchy
	int prevIndexInHierarchy = prevParent->indexOfChild( movedItem );

	// apply the drop event
	QTreeWidget::dropEvent( event );	

	// get new parent item
	CustomTreeItem* newParent = CAST_S( CustomTreeItem*, movedItem->parent() );
	// check if drop is outside scene
	if ( newParent == NULL ) {
		// if so, reset drop
		// remove from tree
		int tmpIndex = this->indexOfTopLevelItem( movedItem );
		this->takeTopLevelItem( tmpIndex );
		// add back to old position
		prevParent->insertChild( prevIndexInHierarchy, movedItem );
		// select item
		this->setCurrentItem( movedItem );
		// exit
		return;
	}

	// select item
	this->setCurrentItem( movedItem );

	// get the order in the new hierarchy
	int indexInHierarchy = newParent->indexOfChild( movedItem );

	// apply changes to gameobjects
	if ( newParent != NULL ) {
		movedItem->node->SetParentNode( NULL );
		newParent->node->InsertChildNode( indexInHierarchy, movedItem->node );
	}
	else {
		movedItem->node->SetParentNode( NULL );
	}
}
