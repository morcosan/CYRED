// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "SelectorPopup.h"
#include "Selectors\Selector.h"
#include "EditorSkin.h"
#include "AttrViewers\AttrViewer.h"

#include "Selectors\Selector_Material.h"
#include "Selectors\Selector_Shader.h"

#include "QtWidgets\qboxlayout.h"
#include "QtWidgets\qlistwidget.h"


using namespace CYRED;



SelectorPopup::SelectorPopup( QWidget* parent )
	: QWidget( parent )
{
	this->setMinimumSize( MIN_SIZE.x, MIN_SIZE.y );
	this->setWindowFlags( Qt::Dialog | Qt::CustomizeWindowHint | 
						  Qt::WindowTitleHint | Qt::WindowCloseButtonHint );
	this->setObjectName( EditorSkin::SELECTOR_POPUP );

	_qtList = Memory::Alloc<QListWidget>( this );
	QObject::connect( _qtList, &QListWidget::itemClicked, this, &SelectorPopup::A_Apply );

	QVBoxLayout* vLayout = Memory::Alloc<QVBoxLayout>();
	vLayout->setSpacing( 5 );
	vLayout->setContentsMargins( 3, 3, 3, 5 );

	vLayout->addWidget( _qtList );

	this->setLayout( vLayout );
}


SelectorPopup::~SelectorPopup()
{
	Iterator<String, Selector*> iter = _selectors.GetIterator();

	while ( iter.HasNext() )
	{
		Memory::Free( iter.GetValue() );
		iter.Next();
	}
}


void SelectorPopup::A_Apply( QListWidgetItem* item )
{
	_ListItem* listItem = CAST_S( _ListItem*, item );

	AttrViewer::QtSelector* qtSelector = CAST_S( AttrViewer::QtSelector*, _qtSelectorPtr );
	qtSelector->OnChangeSelection( listItem->valueRef, listItem->text().toUtf8().constData() );
}


void SelectorPopup::DisplayPopup( const Char* type, void* qtSelectorPtr )
{
	if ( _selectors.Has( type ) )
	{
		_qtSelectorPtr = qtSelectorPtr;
		AttrViewer::QtSelector* qtSelector = CAST_S( AttrViewer::QtSelector*, qtSelectorPtr );
		_selectors.Get( type )->DisplayOptions( qtSelector->selected );
	}

	this->show();
}


void SelectorPopup::Initialize()
{
	AddSelector( Selector_Material::TYPE,	Memory::Alloc<Selector_Material>( this ) );
	AddSelector( Selector_Shader::TYPE,		Memory::Alloc<Selector_Shader>( this ) );
}


void SelectorPopup::AddSelector( const Char* type, Selector* selector )
{
	if ( _selectors.Has( type ) )
	{
		Memory::Free( _selectors.Get( type ) );
	}
	_selectors.Set( type, selector );
}


void SelectorPopup::ClearList()
{
	_qtList->clear();
}


void SelectorPopup::AddToList( const Char* itemName, void* itemRef )
{
	_ListItem* listItem = Memory::Alloc<_ListItem>();
	listItem->setText( itemName );
	listItem->valueRef = itemRef;

	_qtList->addItem( listItem );
}


void SelectorPopup::SetSelected( UInt index )
{
	_qtList->setCurrentRow( index );
}


void SelectorPopup::SetPopupTitle( const Char* title )
{
	this->setWindowTitle( title );
}
