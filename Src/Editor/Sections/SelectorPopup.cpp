// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "SelectorPopup.h"
#include "Selectors\Selector.h"
#include "Settings/EditorSkin.h"
#include "AttrViewers\AttrViewer.h"

#include "Selectors\Selector_Material.h"
#include "Selectors\Selector_Mesh.h"
#include "Selectors\Selector_Morph.h"
#include "Selectors\Selector_Shader.h"
#include "Selectors\Selector_Texture.h"
#include "Selectors\Selector_Scene.h"
#include "Selectors\Selector_Script.h"
#include "Selectors\Selector_Prefab.h"

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

	_qtList = new QListWidget( this );
	QObject::connect( _qtList, &QListWidget::itemClicked, this, &SelectorPopup::A_Apply );

	QVBoxLayout* vLayout = new QVBoxLayout();
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
		PTR_FREE( iter.GetValue() );
		iter.Next();
	}
}


void SelectorPopup::A_Apply( QListWidgetItem* item )
{
	_ListItem* listItem = CAST_S( _ListItem*, item );

	AttrViewer::QtSelector* qtSelector = CAST_S( AttrViewer::QtSelector*, _qtSelectorPtr );
	qtSelector->OnChangeSelection( listItem->valueRef, listItem->text().toUtf8().constData() );
}


void SelectorPopup::DisplayPopup( cchar* type, void* qtSelectorPtr )
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
	AddSelector( Selector_Material::TYPE,	new Selector_Material( this ) );
	AddSelector( Selector_Mesh::TYPE,		new Selector_Mesh( this ) );
	AddSelector( Selector_Morph::TYPE,		new Selector_Morph( this ) );
	AddSelector( Selector_Shader::TYPE,		new Selector_Shader( this ) );
	AddSelector( Selector_Texture::TYPE,	new Selector_Texture( this ) );
	AddSelector( Selector_Scene::TYPE,		new Selector_Scene( this ) );
	AddSelector( Selector_Script::TYPE,		new Selector_Script( this ) );
	AddSelector( Selector_Prefab::TYPE,		new Selector_Prefab( this ) );
}


void SelectorPopup::AddSelector( cchar* type, Selector* selector )
{
	if ( _selectors.Has( type ) )
	{
		PTR_FREE( _selectors.Get( type ) );
	}
	_selectors.Set( type, selector );
}


void SelectorPopup::ClearList()
{
	_qtList->clear();
}


void SelectorPopup::AddToList( cchar* itemName, void* itemRef )
{
	_ListItem* listItem = new _ListItem();
	listItem->setText( itemName );
	listItem->valueRef = itemRef;

	_qtList->addItem( listItem );
}


void SelectorPopup::SetSelected( int index )
{
	_qtList->setCurrentRow( index );
}


void SelectorPopup::SetPopupTitle( cchar* title )
{
	this->setWindowTitle( title );
}
