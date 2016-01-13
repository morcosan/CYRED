// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AttrViewer.h"
#include "../EditorSkin.h"
#include "../../EditorApp.h"

#include "../../Utils/CustomTreeWidget.h"

#include "QtWidgets\qformlayout.h"
#include "QtWidgets\qlineedit.h"
#include "QtWidgets\QCheckBox"
#include "QtWidgets\qboxlayout.h"
#include "QtWidgets\qlabel.h"
#include "QtWidgets\QComboBox"
#include "QtWidgets\qtreewidget.h"
#include "QtWidgets\qgroupbox.h"
#include "QtWidgets\qpushbutton.h"
#include "QtWidgets\qheaderview.h"


using namespace CYRED;



struct AttrViewer::_ListSizeField : public QWidget
{
	CustomTreeWidget*	treeWidget;
	QTreeWidgetItem*	rootItem;
	AttrType			elementType;

	void A_OnSizeChanged()
	{
		QLineEdit* lineEdit = CAST_S( QLineEdit*, this->layout()->itemAt(1)->widget() );
		Int size = lineEdit->text().toInt();
		if ( size < 0 )
		{
			size = 0;
			lineEdit->setText( "0" );
		}

		ASSERT( rootItem != NULL );

		// clear the list
		while ( rootItem->childCount() > 0 )
		{
			QTreeWidgetItem* child = rootItem->takeChild( 0 );
			Memory::Free( child );
		}

		// add new items
		for ( Int i = 0; i < size; ++i )
		{
			Char elementName[MAX_SIZE_CUSTOM_STRING];
			CUSTOM_STRING( elementName, "Element %d", i );

			QTreeWidgetItem* childItem = Memory::Alloc<QTreeWidgetItem>();
			childItem->setFlags( Qt::ItemIsEnabled );
			childItem->setText( 0, elementName );

			QWidget* valueWidget = new QLineEdit();

			switch ( elementType )
			{
				case AttrType::BOOL:
					//valueWidget = _crea
					break;
			}

			rootItem->addChild( childItem );

			treeWidget->setItemWidget( childItem, 1, valueWidget );
		}

		rootItem->setExpanded( (size > 0) );

		treeWidget->A_CalculateHeight();
	}
};


AttrViewer::AttrViewer()
	: _panelTree( NULL )
{
}


void AttrViewer::UpdateGUI()
{
	_OnUpdate_GUI();
}


void AttrViewer::Clear()
{
	_attributes.Clear();
	_groups.Clear();
}


void AttrViewer::RefreshPanel()
{
	_panelTree->setStyleSheet( _panelTree->styleSheet() );
}


void AttrViewer::UsePanel( QTreeWidget* panelTree )
{
	_panelTree = panelTree;
}


void AttrViewer::A_OnChange_Group1()
{
	_activatedGroup = CallbackGroup::GROUP_1;
	_OnUpdate_Target();
}


void AttrViewer::A_OnChange_Group2()
{
	_activatedGroup = CallbackGroup::GROUP_2;
	_OnUpdate_Target();
}


void AttrViewer::QtSelector::A_OnOpenSelector()
{
	EditorApp::Singleton()->ShowSelectorPopup( type.GetChar(), this );
}


void AttrViewer::QtSelector::OnChangeSelection( void* ref, const Char* name )
{
	selected = ref;

	QLineEdit* textWidget = CAST_S( QLineEdit*, this->layout()->itemAt( 0 )->widget() );
	textWidget->setText( name );

	switch ( this->callbackGroup )
	{
		case CallbackGroup::GROUP_1:
			attrViewer->A_OnChange_Group1();
			break;

		case CallbackGroup::GROUP_2:
			attrViewer->A_OnChange_Group2();
			break;
	}
}


void AttrViewer::_AddToPanel( const Char* title )
{
	_titleItem = Memory::Alloc<QTreeWidgetItem>();
	_titleItem->setFlags( Qt::ItemIsEnabled );
	_panelTree->addTopLevelItem( _titleItem );
	_panelTree->expandItem( _titleItem );

	_childItem = Memory::Alloc<QTreeWidgetItem>();
	_childItem->setFlags( Qt::ItemIsEnabled );
	_titleItem->addChild( _childItem );

	// add title
	{
		QHBoxLayout* boxLayout = Memory::Alloc<QHBoxLayout>();
		boxLayout->setAlignment( Qt::AlignLeft );

		{
			if ( _innerAttributes.Has( InnerAttrType::ENABLED ) )
			{
				boxLayout->addWidget( _innerAttributes.Get( InnerAttrType::ENABLED ).valueWidget );
			}
		}

		QLabel* titleLabel = Memory::Alloc<QLabel>( title );
		titleLabel->setObjectName( EditorSkin::ATTR_COMP_TITLE );
		boxLayout->addWidget( titleLabel );

		_titleWidget = Memory::Alloc<QWidget>( _panelTree );
		_titleWidget->setLayout( boxLayout );

		_panelTree->setItemWidget( _titleItem, 0, _titleWidget );
	}

	// add attributes
	{
		// used to display both attributes and groups
		QVBoxLayout* childLayout = Memory::Alloc<QVBoxLayout>();
		childLayout->setSpacing( 5 );
		childLayout->setContentsMargins( 0, 0, 0, 0 );

		_childWidget = Memory::Alloc<QWidget>( _panelTree );
		_childWidget->setLayout( childLayout );

		UInt attrIndex = 0;
		UInt groupIndex = 0;

		// loop until all attributes are displayed
		while ( attrIndex < _attributes.Size() )
		{
			CustomTreeWidget* treeWidget = Memory::Alloc<CustomTreeWidget>();
			treeWidget->setHeaderHidden( true );
			treeWidget->setDragEnabled( false );
			treeWidget->setColumnCount( 2 );
			treeWidget->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
			treeWidget->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
			treeWidget->setObjectName( EditorSkin::ATTR_GROUP_TREE );
			treeWidget->header()->resizeSection( 0, 120 );
			treeWidget->SetAttrViewer( this );

			QVBoxLayout* groupLayout = Memory::Alloc<QVBoxLayout>();
			groupLayout->setContentsMargins( 0, 0, 0, 0 );
			groupLayout->addWidget( treeWidget );

			QGroupBox* groupWidget = Memory::Alloc<QGroupBox>();
			groupWidget->setObjectName( EditorSkin::ATTR_GROUP );
			groupWidget->setLayout( groupLayout );

			childLayout->addWidget( groupWidget );


			UInt first = attrIndex;
			UInt last = _attributes.Size() - 1;

			if ( groupIndex < _groups.Size() )
			{
				_Group currGroup = _groups[groupIndex];
				UInt firstIndex = currGroup.firstIndex;

				if ( firstIndex == attrIndex )
				{
					first = currGroup.firstIndex;
					last = first + currGroup.count - 1;
					++ groupIndex;

					groupWidget->setTitle( currGroup.name.GetChar() );
				}
				else
				{
					last = firstIndex - 1;
				}
			}

			while ( first <= attrIndex && attrIndex <= last )
			{
				QTreeWidgetItem* item = Memory::Alloc<QTreeWidgetItem>();
				item->setFlags( Qt::ItemIsEnabled );

				treeWidget->addTopLevelItem( item );

				// set the attr name
				item->setText( 0, _attributes[attrIndex].name.GetChar() );

				// set the attr value
				treeWidget->setItemWidget( item, 1, _attributes[attrIndex].valueWidget );

				_attributes[attrIndex].treeItem = item;
				_attributes[attrIndex].treeWidget = treeWidget;

				if ( _attributes[attrIndex].type == AttrType::LIST )
				{
					_ListSizeField* sizeField = CAST_S( _ListSizeField*, _attributes[attrIndex].valueWidget );
					sizeField->rootItem = item;
					sizeField->treeWidget = treeWidget;
				}

				++ attrIndex;
			}

			treeWidget->A_CalculateHeight();
		}

		_panelTree->setItemWidget( _childItem, 0, _childWidget );
	}
}


void AttrViewer::_UpdateVisibility()
{
	for ( UInt i = 0; i < _attributes.Size(); ++i )
	{
		_attributes[i].treeItem->setHidden( !_attributes[i].isVisible );
		_attributes[i].treeWidget->A_CalculateHeight();
	}

	RefreshPanel();
}


void AttrViewer::_OpenGroup( const Char* name )
{
	_groups.Add( _Group{ name, _attributes.Size(), 0 } );
}


void AttrViewer::_CloseGroup()
{
	UInt size = _groups.Size();
	ASSERT( size > 0 );

	_groups[ size - 1 ].count = _attributes.Size() - _groups[ size - 1 ].firstIndex;
}


void AttrViewer::_CreateAttrString( const Char* name )
{
	QWidget* widget = _CreateString( AttrFlag::NONE, CallbackGroup::GROUP_1 );
	_SetAttribute( name, widget, AttrType::STRING );
}


void AttrViewer::_CreateAttrBool( const Char* name )
{
	QWidget* widget = _CreateBool( AttrFlag::NONE, CallbackGroup::GROUP_1 );
	_SetAttribute( name, widget, AttrType::BOOL );
}


void AttrViewer::_CreateAttrInt( const Char* name )
{
	QWidget* widget = _CreateInt( AttrFlag::NONE, CallbackGroup::GROUP_1 );
	_SetAttribute( name, widget, AttrType::INT );
}


void AttrViewer::_CreateAttrFloat( const Char* name )
{
	QWidget* widget = _CreateFloat( AttrFlag::NONE, CallbackGroup::GROUP_1 );
	_SetAttribute( name, widget, AttrType::FLOAT );
}


void AttrViewer::_CreateAttrVector2( const Char* name )
{
	QWidget* widget = _CreateVector2( AttrFlag::NONE, CallbackGroup::GROUP_1 );
	_SetAttribute( name, widget, AttrType::VECTOR2 );
}


void AttrViewer::_CreateAttrVector3( const Char* name )
{
	QWidget* widget = _CreateVector3( AttrFlag::NONE, CallbackGroup::GROUP_1 );
	_SetAttribute( name, widget, AttrType::VECTOR3 );
}


void AttrViewer::_CreateAttrVector4( const Char* name )
{
	QWidget* widget = _CreateVector4( AttrFlag::NONE, CallbackGroup::GROUP_1 );
	_SetAttribute( name, widget, AttrType::VECTOR4 );
}


void AttrViewer::_CreateAttrDropdown( const Char* name, DataArray<const Char*>& valueList )
{
	QWidget* widget = _CreateDropdown( valueList, AttrFlag::NONE, CallbackGroup::GROUP_1 );
	_SetAttribute( name, widget, AttrType::DROPDOWN );
}


void AttrViewer::_CreateAttrSelector( const Char* name, const Char* dataType )
{
	QWidget* widget = _CreateSelector( dataType, AttrFlag::NONE, CallbackGroup::GROUP_1 );
	_SetAttribute( name, widget, AttrType::SELECTOR );
}


void AttrViewer::_CreateAttrList( const Char* name, AttrType elementType )
{
	QWidget* widget = _CreateList( elementType, AttrFlag::NONE, CallbackGroup::GROUP_1 );
	_SetAttribute( name, widget, AttrType::LIST );
}


void AttrViewer::_CreateAttrString( const Char* name, UInt flagMask, CallbackGroup group )
{
	QWidget* widget = _CreateString( flagMask, group );
	_SetAttribute( name, widget, AttrType::STRING );
}


void AttrViewer::_CreateAttrBool( const Char* name, UInt flagMask, CallbackGroup group )
{
	QWidget* widget = _CreateBool( flagMask, group );
	_SetAttribute( name, widget, AttrType::BOOL );
}


void AttrViewer::_CreateAttrInt( const Char* name, UInt flagMask, CallbackGroup group )
{
	QWidget* widget = _CreateInt( flagMask, group );
	_SetAttribute( name, widget, AttrType::INT );
}


void AttrViewer::_CreateAttrFloat( const Char* name, UInt flagMask, CallbackGroup group )
{
	QWidget* widget = _CreateFloat( flagMask, group );
	_SetAttribute( name, widget, AttrType::FLOAT );
}


void AttrViewer::_CreateAttrVector2( const Char* name, UInt flagMask, CallbackGroup group )
{
	QWidget* widget = _CreateVector2( flagMask, group );
	_SetAttribute( name, widget, AttrType::VECTOR2 );
}


void AttrViewer::_CreateAttrVector3( const Char* name, UInt flagMask, CallbackGroup group )
{
	QWidget* widget = _CreateVector3( flagMask, group );
	_SetAttribute( name, widget, AttrType::VECTOR3 );
}


void AttrViewer::_CreateAttrVector4( const Char* name, UInt flagMask, CallbackGroup group )
{
	QWidget* widget = _CreateVector4( flagMask, group );
	_SetAttribute( name, widget, AttrType::VECTOR4 );
}


void AttrViewer::_CreateAttrDropdown( const Char* name, DataArray<const Char*>& valueList, 
									  UInt flagMask, CallbackGroup group )
{
	QWidget* widget = _CreateDropdown( valueList, flagMask, group );
	_SetAttribute( name, widget, AttrType::DROPDOWN );
}


void AttrViewer::_CreateAttrSelector( const Char* name, const Char* dataType, 
									  UInt flagMask, CallbackGroup group )
{
	QWidget* widget = _CreateSelector( dataType, flagMask, group );
	_SetAttribute( name, widget, AttrType::SELECTOR );
}


DataUnion AttrViewer::_ReadAttribute( const Char* name )
{
	_Attribute attr;
	Bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );

	return _ReadAttr( attr );
}


void AttrViewer::_WriteAttribute( const Char* name, DataUnion& attrValue )
{
	_Attribute attr;
	Bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );

	_WriteAttr( attr, attrValue, NULL );
}


void AttrViewer::_WriteAttribute( const Char* name, DataUnion & attrValue, 
								  const Char* attrValueName )
{
	_Attribute attr;
	Bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );

	_WriteAttr( attr, attrValue, attrValueName );
}


void AttrViewer::_CreateInnerAttribute( InnerAttrType innerType )
{
	_InnerAttribute attr;

	switch ( innerType )
	{
		case InnerAttrType::ENABLED:
			attr.type = AttrType::BOOL;
			attr.valueWidget = _CreateBool( AttrFlag::NONE, CallbackGroup::GROUP_1 );
			break;
	}

	_innerAttributes.Set( innerType, attr );
}


DataUnion AttrViewer::_ReadInnerAttribute( InnerAttrType innerType )
{
	ASSERT( _innerAttributes.Has( innerType ) );

	_InnerAttribute attr = _innerAttributes.Get( innerType );

	DataUnion attrValue;

	switch ( attr.type )
	{
		case AttrType::BOOL:
			attrValue.SetBool( _ReadBool( attr.valueWidget ) );
			break;
	}

	return attrValue;
}


void AttrViewer::_WriteInnerAttribute( InnerAttrType innerType, DataUnion& attrValue )
{
	ASSERT( _innerAttributes.Has( innerType ) );

	_InnerAttribute attr = _innerAttributes.Get( innerType );

	switch ( attr.type )
	{
		case AttrType::BOOL:
			_WriteBool( attr.valueWidget, attrValue.GetBool() );
			break;
	}
}


void AttrViewer::_SetAttrVisibility( const Char* name, Bool value )
{
	for ( UInt i = 0; i < _attributes.Size(); ++i )
	{
		if ( _attributes[i].name == name )
		{
			_attributes[i].isVisible = value;
			return;
		}
	}

	ASSERT( FALSE );
}


void AttrViewer::_Colorize( Bool enabled, Bool colorizeAll )
{
	ASSERT( _panelTree != NULL );

	if ( colorizeAll )
	{
		_panelTree->setProperty( EditorSkin::FLAG_DISABLED, !enabled );
	}
	else
	{
		_titleWidget->setProperty( EditorSkin::FLAG_DISABLED, !enabled );
		_childWidget->setProperty( EditorSkin::FLAG_DISABLED, !enabled );
	}
	RefreshPanel();
}


void AttrViewer::_SetAttribute( const Char* name, QWidget* widget, AttrType type )
{
	for ( UInt i = 0; i < _attributes.Size(); ++i )
	{
		if ( _attributes[i].name == name )
		{
			_attributes[i].valueWidget = widget;
			_attributes[i].type = type;
			return;
		}
	}

	//! is a new element
	_attributes.Add( _Attribute{ name, NULL, NULL, widget, type, TRUE } );
}


Bool AttrViewer::_GetAttribute( const Char* name, OUT _Attribute& attribute)
{
	for ( UInt i = 0; i < _attributes.Size(); ++i )
	{
		if ( _attributes[i].name == name )
		{
			attribute = _attributes[i];
			return TRUE;
		}
	}

	return FALSE;
}


DataUnion AttrViewer::_ReadAttr( _Attribute attr )
{
	DataUnion attrValue;

	switch ( attr.type )
	{
		case AttrType::BOOL:
			attrValue.SetBool( _ReadBool( attr.valueWidget ) );
			break;

		case AttrType::FLOAT:
			attrValue.SetFloat( _ReadFloat( attr.valueWidget ) );
			break;

		case AttrType::INT:
			attrValue.SetInt( _ReadInt( attr.valueWidget ) );
			break;

		case AttrType::STRING:
			attrValue.SetString( _ReadString( attr.valueWidget ).GetChar() );
			break;

		case AttrType::VECTOR2:
			attrValue.SetVector2( _ReadVector2( attr.valueWidget ) );
			break;

		case AttrType::VECTOR3:
			attrValue.SetVector3( _ReadVector3( attr.valueWidget ) );
			break;

		case AttrType::VECTOR4:
			attrValue.SetVector4( _ReadVector4( attr.valueWidget ) );
			break;

		case AttrType::DROPDOWN:
			attrValue.SetInt( _ReadDropdown( attr.valueWidget ) );
			break;

		case AttrType::SELECTOR:
			attrValue.SetReference( _ReadSelector( attr.valueWidget ) );
			break;
	}

	return attrValue;
}


void AttrViewer::_WriteAttr( _Attribute attr, DataUnion& attrValue, 
							 const Char* attrValueName )
{
	switch ( attr.type )
	{
		case AttrType::BOOL:
			_WriteBool( attr.valueWidget, attrValue.GetBool() );
			break;

		case AttrType::FLOAT:
			_WriteFloat( attr.valueWidget, attrValue.GetFloat() );
			break;

		case AttrType::INT:
			_WriteInt( attr.valueWidget, attrValue.GetInt() );
			break;

		case AttrType::STRING:
			_WriteString( attr.valueWidget, attrValue.GetString() );
			break;

		case AttrType::VECTOR2:
			_WriteVector2( attr.valueWidget, attrValue.GetVector2() );
			break;

		case AttrType::VECTOR3:
			_WriteVector3( attr.valueWidget, attrValue.GetVector3() );
			break;

		case AttrType::VECTOR4:
			_WriteVector4( attr.valueWidget, attrValue.GetVector4() );
			break;

		case AttrType::DROPDOWN:
			_WriteDropdown( attr.valueWidget, attrValue.GetInt() );
			break;

		case AttrType::SELECTOR:
			_WriteSelector( attr.valueWidget, attrValue.GetReference(), attrValueName );
			break;
	}
}


QWidget* AttrViewer::_CreateString( UInt flagMask, CallbackGroup group )
{
	QLineEdit* widget = Memory::Alloc<QLineEdit>();
	widget->setReadOnly( (flagMask & AttrFlag::READONLY) != 0 );

	switch ( group )
	{
		case CallbackGroup::GROUP_1:
			if ( (flagMask & AttrFlag::EDIT_FINISH) != 0 )
			{
				QObject::connect( widget, &QLineEdit::editingFinished, 
								  this, &AttrViewer::A_OnChange_Group1 );
			}
			else
			{
				QObject::connect( widget, &QLineEdit::textEdited, 
								  this, &AttrViewer::A_OnChange_Group1 );
			}
			break;

		case CallbackGroup::GROUP_2:
			if ( (flagMask & AttrFlag::EDIT_FINISH) != 0 )
			{
				QObject::connect( widget, &QLineEdit::editingFinished, 
								  this, &AttrViewer::A_OnChange_Group2 );
			}
			else
			{
				QObject::connect( widget, &QLineEdit::textEdited, 
								  this, &AttrViewer::A_OnChange_Group2 );
			}
			break;
	}

	return widget;
}


QWidget* AttrViewer::_CreateFloat( UInt flagMask, CallbackGroup group )
{
	QLineEdit* widget = Memory::Alloc<QLineEdit>();
	widget->setValidator( Memory::Alloc<QDoubleValidator>( widget ) );
	widget->setReadOnly( (flagMask & AttrFlag::READONLY) != 0 );

	switch ( group )
	{
		case CallbackGroup::GROUP_1:
			QObject::connect( widget, &QLineEdit::textEdited, this, &AttrViewer::A_OnChange_Group1 );
			break;

		case CallbackGroup::GROUP_2:
			QObject::connect( widget, &QLineEdit::textEdited, this, &AttrViewer::A_OnChange_Group2 );
			break;
	}

	return widget;
}


QWidget* AttrViewer::_CreateInt( UInt flagMask, CallbackGroup group )
{
	QLineEdit* widget = Memory::Alloc<QLineEdit>();
	widget->setValidator( Memory::Alloc<QIntValidator>( widget ) );
	widget->setReadOnly( (flagMask & AttrFlag::READONLY) != 0 );

	switch ( group )
	{
		case CallbackGroup::GROUP_1:
			QObject::connect( widget, &QLineEdit::textEdited, this, &AttrViewer::A_OnChange_Group1 );
			break;

		case CallbackGroup::GROUP_2:
			QObject::connect( widget, &QLineEdit::textEdited, this, &AttrViewer::A_OnChange_Group2 );
			break;
	}

	return widget;
}


QWidget* AttrViewer::_CreateBool( UInt flagMask, CallbackGroup group )
{
	QCheckBox* widget = Memory::Alloc<QCheckBox>();
	widget->setDisabled( (flagMask & AttrFlag::READONLY) != 0 );


	switch ( group )
	{
		case CallbackGroup::GROUP_1:
		{
			QObject::connect( widget, &QCheckBox::clicked, this, &AttrViewer::A_OnChange_Group1 );
			break;
		}

		case CallbackGroup::GROUP_2:
		{
			QObject::connect( widget, &QCheckBox::clicked, this, &AttrViewer::A_OnChange_Group2 );
			break;
		}
	}

	return widget;
}


QWidget* AttrViewer::_CreateVector2( UInt flagMask, CallbackGroup group )
{
	QHBoxLayout* boxLayout = Memory::Alloc<QHBoxLayout>();
	boxLayout->setAlignment( Qt::AlignLeft );
	boxLayout->setSpacing( 0 );
	boxLayout->setContentsMargins( 0, 0, 0, 0 );

	boxLayout->addWidget( Memory::Alloc<QLabel>( " X" ) );
	boxLayout->addWidget( _CreateFloat( flagMask, group ) );
	boxLayout->addWidget( Memory::Alloc<QLabel>( " Y" ) );
	boxLayout->addWidget( _CreateFloat( flagMask, group ) );

	QWidget* widget = Memory::Alloc<QWidget>();
	widget->setLayout( boxLayout );

	return widget;
}


QWidget* AttrViewer::_CreateVector3( UInt flagMask, CallbackGroup group )
{
	QHBoxLayout* boxLayout = Memory::Alloc<QHBoxLayout>();
	boxLayout->setAlignment( Qt::AlignLeft );
	boxLayout->setSpacing( 0 );
	boxLayout->setContentsMargins( 0, 0, 0, 0 );

	boxLayout->addWidget( Memory::Alloc<QLabel>( " X" ) );
	boxLayout->addWidget( _CreateFloat( flagMask, group ) );
	boxLayout->addWidget( Memory::Alloc<QLabel>( " Y" ) );
	boxLayout->addWidget( _CreateFloat( flagMask, group ) );
	boxLayout->addWidget( Memory::Alloc<QLabel>( " Z" ) );
	boxLayout->addWidget( _CreateFloat( flagMask, group ) );


	QWidget* widget = Memory::Alloc<QWidget>();
	widget->setLayout( boxLayout );

	return widget;
}


QWidget* AttrViewer::_CreateVector4( UInt flagMask, CallbackGroup group )
{
	QHBoxLayout* boxLayout = Memory::Alloc<QHBoxLayout>();
	boxLayout->setAlignment( Qt::AlignLeft );
	boxLayout->setSpacing( 0 );
	boxLayout->setContentsMargins( 0, 0, 0, 0 );

	boxLayout->addWidget( Memory::Alloc<QLabel>( " X" ) );
	boxLayout->addWidget( _CreateFloat( flagMask, group ) );
	boxLayout->addWidget( Memory::Alloc<QLabel>( " Y" ) );
	boxLayout->addWidget( _CreateFloat( flagMask, group ) );
	boxLayout->addWidget( Memory::Alloc<QLabel>( " Z" ) );
	boxLayout->addWidget( _CreateFloat( flagMask, group ) );
	boxLayout->addWidget( Memory::Alloc<QLabel>( " W" ) );
	boxLayout->addWidget( _CreateFloat( flagMask, group ) );

	QWidget* widget = Memory::Alloc<QWidget>();
	widget->setLayout( boxLayout );

	return widget;
}


QWidget* AttrViewer::_CreateDropdown( DataArray<const Char*>& valueList, UInt flagMask,
									  CallbackGroup group )
{
	QStringList stringList;
	for ( UInt i = 0; i < valueList.Size(); ++i )
	{
		stringList << valueList[i];
	}

	QComboBox* widget = Memory::Alloc<QComboBox>();
	widget->addItems( stringList );

	switch ( group )
	{
		case CallbackGroup::GROUP_1:
		{
			QObject::connect( widget, &QComboBox::currentTextChanged, this, &AttrViewer::A_OnChange_Group1 );
			break;
		}

		case CallbackGroup::GROUP_2:
		{
			QObject::connect( widget, &QComboBox::currentTextChanged, this, &AttrViewer::A_OnChange_Group2 );
			break;
		}
	}

	return widget;
}


QWidget* AttrViewer::_CreateSelector( const Char* dataType, UInt flagMask, CallbackGroup group )
{
	QHBoxLayout* boxLayout = Memory::Alloc<QHBoxLayout>();
	boxLayout->setAlignment( Qt::AlignLeft );
	boxLayout->setSpacing( 0 );
	boxLayout->setContentsMargins( 0, 0, 0, 0 );

	QLineEdit* textWidget = Memory::Alloc<QLineEdit>();
	textWidget->setReadOnly( true );

	QPushButton* button = Memory::Alloc<QPushButton>();
	button->setText( "*" );
	button->setObjectName( EditorSkin::ATTR_SELECTOR_BUTTON );

	boxLayout->addWidget( textWidget );
	boxLayout->addWidget( button );

	QtSelector* widget = Memory::Alloc<QtSelector>();
	widget->setLayout( boxLayout );
	widget->type = dataType;
	widget->selected = NULL;
	widget->callbackGroup = group;
	widget->attrViewer = this;

	QObject::connect( button, &QPushButton::clicked, 
					  widget, &QtSelector::A_OnOpenSelector );

	return widget;
}


QWidget* AttrViewer::_CreateList( AttrType elementType, UInt flagMask, CallbackGroup group )
{
	QLineEdit* lineEdit = Memory::Alloc<QLineEdit>();
	lineEdit->setValidator( Memory::Alloc<QIntValidator>( lineEdit ) );
	lineEdit->setReadOnly( (flagMask & AttrFlag::READONLY) != 0 );
	lineEdit->setText( "0" );

	switch ( group )
	{
		case CallbackGroup::GROUP_1:
			QObject::connect( lineEdit, &QLineEdit::textEdited, this, &AttrViewer::A_OnChange_Group1 );
			break;

		case CallbackGroup::GROUP_2:
			QObject::connect( lineEdit, &QLineEdit::textEdited, this, &AttrViewer::A_OnChange_Group2 );
			break;
	}

	QHBoxLayout* boxLayout = Memory::Alloc<QHBoxLayout>();
	boxLayout->setAlignment( Qt::AlignLeft );
	boxLayout->setSpacing( 0 );
	boxLayout->setContentsMargins( 0, 0, 0, 0 );

	boxLayout->addWidget( Memory::Alloc<QLabel>( "[..]" ) );
	boxLayout->addWidget( lineEdit );

	_ListSizeField* widget = Memory::Alloc<_ListSizeField>();
	widget->setLayout( boxLayout );
	widget->elementType = elementType;

	QObject::connect( lineEdit, &QLineEdit::textEdited, widget, &_ListSizeField::A_OnSizeChanged );

	return widget;
}


void AttrViewer::_WriteString( QWidget* widget, const Char* value )
{
	QLineEdit* textWidget = CAST_S( QLineEdit*, widget );
	textWidget->blockSignals( true );
	textWidget->setText( value );
	textWidget->blockSignals( false );
}


void AttrViewer::_WriteFloat( QWidget* widget, Float value )
{
	QLineEdit* textWidget = CAST_S( QLineEdit*, widget );
	textWidget->blockSignals( true );
	textWidget->setText( QString::number( value ) );
	textWidget->blockSignals( false );
}


void AttrViewer::_WriteInt( QWidget* widget, Int value )
{
	QLineEdit* textWidget = CAST_S( QLineEdit*, widget );
	textWidget->blockSignals( true );
	textWidget->setText( QString::number( value ) );
	textWidget->blockSignals( false );
}


void AttrViewer::_WriteBool( QWidget* widget, Bool value )
{
	CAST_S( QCheckBox*, widget )->setChecked( value );
}


void AttrViewer::_WriteVector2( QWidget* widget, const Vector2& value )
{
	QLineEdit* xWidget;
	QLineEdit* yWidget;

	xWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 1 )->widget() );
	xWidget->blockSignals( true );
	xWidget->setText( QString::number( value.x ) );
	xWidget->blockSignals( false );

	yWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 3 )->widget() );
	yWidget->blockSignals( true );
	yWidget->setText( QString::number( value.y ) );
	yWidget->blockSignals( false );
}


void AttrViewer::_WriteVector3( QWidget* widget, const Vector3& value )
{
	QLineEdit* xWidget;
	QLineEdit* yWidget;
	QLineEdit* zWidget;

	xWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 1 )->widget() );
	xWidget->blockSignals( true );
	xWidget->setText( QString::number( value.x ) );
	xWidget->blockSignals( false );

	yWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 3 )->widget() );
	yWidget->blockSignals( true );
	yWidget->setText( QString::number( value.y ) );
	yWidget->blockSignals( false );

	zWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 5 )->widget() );
	zWidget->blockSignals( true );
	zWidget->setText( QString::number( value.z ) );
	zWidget->blockSignals( false );
}


void AttrViewer::_WriteVector4( QWidget* widget, const Vector4& value )
{
	QLineEdit* xWidget;
	QLineEdit* yWidget;
	QLineEdit* zWidget;

	xWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 1 )->widget() );
	xWidget->blockSignals( true );
	xWidget->setText( QString::number( value.x ) );
	xWidget->blockSignals( false );

	yWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 3 )->widget() );
	yWidget->blockSignals( true );
	yWidget->setText( QString::number( value.y ) );
	yWidget->blockSignals( false );

	zWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 5 )->widget() );
	zWidget->blockSignals( true );
	zWidget->setText( QString::number( value.z ) );
	zWidget->blockSignals( false );
}


void AttrViewer::_WriteDropdown( QWidget * widget, UInt valueIndex )
{
	QComboBox* comboBox = CAST_S( QComboBox*, widget );
	comboBox->blockSignals( true );
	comboBox->setCurrentIndex( valueIndex );
	comboBox->blockSignals( false );
}


void AttrViewer::_WriteSelector( QWidget* widget, void* valueRef, const Char* valueName )
{
	widget->blockSignals( true );

	QtSelector* selector = CAST_S( QtSelector*, widget );
	selector->selected = valueRef;

	QLineEdit* textWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 0 )->widget() );
	textWidget->setText( valueName );

	widget->blockSignals( false );
}


String AttrViewer::_ReadString( QWidget* widget )
{
	return CAST_S( QLineEdit*, widget )->text().toUtf8().constData();
}


Float AttrViewer::_ReadFloat( QWidget* widget )
{
	return CAST_S( QLineEdit*, widget )->text().toFloat();
}


Int AttrViewer::_ReadInt( QWidget* widget )
{
	return CAST_S( QLineEdit*, widget )->text().toInt();
}


Bool AttrViewer::_ReadBool( QWidget* widget )
{
	return CAST_S( QCheckBox*, widget )->isChecked();
}


Vector2 AttrViewer::_ReadVector2( QWidget* widget )
{
	QLineEdit* xWidget = CAST_S( QLineEdit*, widget->layout()->itemAt(1)->widget() );
	QLineEdit* yWidget = CAST_S( QLineEdit*, widget->layout()->itemAt(3)->widget() );
	float x = xWidget->text().toFloat();
	float y = yWidget->text().toFloat();

	return Vector2( x, y );
}


Vector3 AttrViewer::_ReadVector3( QWidget* widget )
{
	QLineEdit* xWidget = CAST_S( QLineEdit*, widget->layout()->itemAt(1)->widget() );
	QLineEdit* yWidget = CAST_S( QLineEdit*, widget->layout()->itemAt(3)->widget() );
	QLineEdit* zWidget = CAST_S( QLineEdit*, widget->layout()->itemAt(5)->widget() );
	float x = xWidget->text().toFloat();
	float y = yWidget->text().toFloat();
	float z = zWidget->text().toFloat();

	return Vector3( x, y, z );
}


Vector4 AttrViewer::_ReadVector4( QWidget* widget )
{
	QLineEdit* xWidget = CAST_S( QLineEdit*, widget->layout()->itemAt(1)->widget() );
	QLineEdit* yWidget = CAST_S( QLineEdit*, widget->layout()->itemAt(3)->widget() );
	QLineEdit* zWidget = CAST_S( QLineEdit*, widget->layout()->itemAt(5)->widget() );
	QLineEdit* wWidget = CAST_S( QLineEdit*, widget->layout()->itemAt(7)->widget() );
	float x = xWidget->text().toFloat();
	float y = yWidget->text().toFloat();
	float z = zWidget->text().toFloat();
	float w = wWidget->text().toFloat();

	return Vector4( x, y, z, w );
}


UInt AttrViewer::_ReadDropdown( QWidget* widget )
{
	return CAST_S( QComboBox*, widget )->currentIndex();
}


void* AttrViewer::_ReadSelector( QWidget* widget )
{
	return CAST_S( QtSelector*, widget )->selected;
}


