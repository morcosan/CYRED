// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Panel_Hierarchy.h"

#include "../Settings/EditorSkin.h"
#include "../../Utils/CustomTree.h"
#include "../../Utils/CustomTreeItem.h"


using namespace CYRED;


void Panel_Hierarchy::Initialize()
{
	this->setWindowTitle( _GetPanelTitle() );
	this->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
	this->setAllowedAreas( Qt::DockWidgetArea::AllDockWidgetAreas );
	this->setMinimumSize( MIN_SIZE.x, MIN_SIZE.y );

	_qtTree = Memory::Alloc<CustomTree>();
	_qtTree->setHeaderHidden( true );
	_qtTree->setDragEnabled( true );
	_qtTree->setDragDropMode( QAbstractItemView::InternalMove );
	_qtTree->setObjectName( EditorSkin::HIERARCHY_TREE );
	_qtTree->setEditTriggers( QAbstractItemView::NoEditTriggers );

	this->setWidget( _qtTree );

	QObject::connect( _qtTree, &QTreeWidget::itemPressed, this, &Panel_Hierarchy::A_ItemClicked );
	QObject::connect( _qtTree, &QTreeWidget::itemChanged, this, &Panel_Hierarchy::A_ItemRenamed );

	// callback
	_OnInitialize();
}


void Panel_Hierarchy::_RecResetHierarchy( Node* node, QTreeWidgetItem* parentItem )
{
	ASSERT( node != NULL );

	CustomTreeItem* treeItem = Memory::Alloc<CustomTreeItem>();
	treeItem->node = node;

	treeItem->setText( 0, node->GetName() );
	treeItem->setFlags( Qt::ItemIsSelectable |
						Qt::ItemIsEditable |
						Qt::ItemIsDragEnabled |
						Qt::ItemIsDropEnabled |
						Qt::ItemIsEnabled );
	parentItem->addChild( treeItem );

	for ( int i = 0; i < node->GetChildNodeCount(); ++i ) {
		_RecResetHierarchy( node->GetChildNodeAt(i), treeItem );
	}
}


void Panel_Hierarchy::A_ItemRenamed( QTreeWidgetItem* item, int column )
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, item );

	if ( treeItem->asset != NULL ) {
		FiniteString newName( item->text(0).toUtf8().constData() );

		_qtTree->blockSignals( true );
		item->setText( 0, treeItem->asset->GetName() );
		_qtTree->blockSignals( false );

		treeItem->asset->SetName( newName.GetChar() );
	}
	else {
		treeItem->node->SetName( item->text( 0 ).toUtf8().data() );
	}
}