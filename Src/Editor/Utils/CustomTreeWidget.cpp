// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "CustomTreeWidget.h"
#include "../Sections/AttrViewers/AttrViewer.h"

#include "QtWidgets\qheaderview.h"


using namespace CYRED;



CustomTreeWidget::CustomTreeWidget()
{
	QObject::connect( this, &QTreeWidget::itemExpanded, this, &CustomTreeWidget::A_CalculateHeight );
	QObject::connect( this, &QTreeWidget::itemCollapsed, this, &CustomTreeWidget::A_CalculateHeight );
}


void CustomTreeWidget::A_CalculateHeight()
{
	int h = 0;
	int topLevelCount = this->topLevelItemCount();

	for ( int i = 0; i < topLevelCount; ++i )
	{
		QTreeWidgetItem* item = this->topLevelItem(i);
		h += _CalculateHeightRec( item );
	}

	this->setMinimumHeight( h );
	this->setMaximumHeight( h );

	if ( _attrViewer != NULL )
	{
		_attrViewer->RefreshPanel();
	}
}


void CustomTreeWidget::SetAttrViewer( AttrViewer* attrViewer )
{
	_attrViewer = attrViewer;
}


int CustomTreeWidget::_CalculateHeightRec( QTreeWidgetItem* item )
{
	if ( item == NULL )
	{
		return 0;
	}

	QModelIndex index = this->indexFromItem(item);

	if ( !item->isExpanded() )
	{
		return this->rowHeight( index );
	}

	int h = this->rowHeight( index );
	//int h = item->sizeHint(0).height() + 2;
	int childCount = item->childCount();

	for ( int i = 0; i < childCount; ++i )
	{
		h += _CalculateHeightRec( item->child(i) );
	}

	return h;
}