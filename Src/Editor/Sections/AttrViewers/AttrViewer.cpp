// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "AttrViewer.h"
#include "../Settings/EditorSkin.h"
#include "../../EditorApp.h"
#include "../Panels/Panel_Attributes.h"

#include "../../Utils/CustomTreeWidget.h"
#include "../../Utils/QtUtils.h"

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



struct AttrViewer::_StructWidget : public QWidget
{
	DataArray<AttrStruct>	structScheme;
	int					flagMask;
	CallbackGroup			callbackGroup;
	QTreeWidgetItem*		rootItem;
	CustomTreeWidget*		treeWidget;
};


struct AttrViewer::_ListWidget : public QWidget
{
	CustomTreeWidget*		treeWidget;
	QTreeWidgetItem*		rootItem;
	AttrType				elementType;
	AttrViewer*				attrViewer;
	int					flagMask;
	CallbackGroup			callbackGroup;

	cchar*				selectorDataType;
	DataArray<cchar*>  dropdownValues;
	DataArray<AttrStruct>	structScheme;


	void A_OnSizeChanged()
	{
		QLineEdit* lineEdit = CAST_S( QLineEdit*, this->layout()->itemAt(1)->widget() );
		int size = lineEdit->text().toInt();
		if ( size <= 0 )
		{
			size = 0;
			lineEdit->setText( "0" );
		}

		ASSERT( rootItem != NULL );

		// delete if too many
		while ( rootItem->childCount() > size )
		{
			QTreeWidgetItem* child = rootItem->takeChild( rootItem->childCount() - 1 );
			Memory::Free( child );
		}

		// add if not enough
		int childrenLeft = rootItem->childCount();

		for ( int index = childrenLeft; index < size; index++ )
		{
			FiniteString elementName( "[ %d ]", index );

			QTreeWidgetItem* childItem = Memory::Alloc<QTreeWidgetItem>();
			childItem->setFlags( Qt::ItemIsEnabled );
			childItem->setText( 0, elementName.GetChar() );

			QWidget* valueWidget = NULL;

			switch ( elementType )
			{
				case AttrType::STRING:
					valueWidget = attrViewer->CreateString( flagMask, callbackGroup );
					break;

				case AttrType::BOOL:
					valueWidget = attrViewer->CreateBool( flagMask, callbackGroup );
					break;

				case AttrType::INT:
					valueWidget = attrViewer->CreateInt( flagMask, callbackGroup );
					break;

				case AttrType::FLOAT:
					valueWidget = attrViewer->CreateFloat( flagMask, callbackGroup );
					break;

				case AttrType::VECTOR2:
					valueWidget = attrViewer->CreateVector2( flagMask, callbackGroup );
					break;

				case AttrType::VECTOR3:
					valueWidget = attrViewer->CreateVector3( flagMask, callbackGroup );
					break;

				case AttrType::VECTOR4:
					valueWidget = attrViewer->CreateVector4( flagMask, callbackGroup );
					break;

				case AttrType::DROPDOWN:
					valueWidget = attrViewer->CreateDropdown( dropdownValues, flagMask, callbackGroup );
					break;

				case AttrType::SELECTOR:
					valueWidget = attrViewer->CreateSelector( selectorDataType, flagMask, callbackGroup );
					break;

				case AttrType::STRUCT:
				{
					valueWidget = attrViewer->CreateStruct( structScheme, flagMask, callbackGroup );
					_StructWidget* structWidget = CAST_S( _StructWidget*, valueWidget );

					for ( int i = 0; i < structScheme.Size(); ++i )
					{
						QTreeWidgetItem* structItem = Memory::Alloc<QTreeWidgetItem>();
						structItem->setFlags( Qt::ItemIsEnabled );
						structItem->setText( 0, structScheme[i].attrName.GetChar() );

						childItem->addChild( structItem );

						QWidget* elemWidget = NULL;
						switch ( structScheme[i].attrType )
						{
							case AttrType::STRING:
								elemWidget = attrViewer->CreateString( flagMask, callbackGroup );
								break;

							case AttrType::BOOL:
								elemWidget = attrViewer->CreateBool( flagMask, callbackGroup );
								break;

							case AttrType::INT:
								elemWidget = attrViewer->CreateInt( flagMask, callbackGroup );
								break;

							case AttrType::FLOAT:
								elemWidget = attrViewer->CreateFloat( flagMask, callbackGroup );
								break;

							case AttrType::VECTOR2:
								elemWidget = attrViewer->CreateVector2( flagMask, callbackGroup );
								break;

							case AttrType::VECTOR3:
								elemWidget = attrViewer->CreateVector3( flagMask, callbackGroup );
								break;

							case AttrType::VECTOR4:
								elemWidget = attrViewer->CreateVector4( flagMask, callbackGroup );
								break;

							case AttrType::DROPDOWN:
								elemWidget = attrViewer->CreateDropdown( structScheme[i].dropdownValues, 
																		 flagMask, 
																		 callbackGroup );
								break;

							case AttrType::SELECTOR:
								elemWidget = attrViewer->CreateSelector( structScheme[i].selectorDataType.GetChar(), 
																		 flagMask, 
																		 callbackGroup );
								break;
						}

						treeWidget->setItemWidget( structItem, 1, elemWidget );
					}
					break;
				}
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
	, _ignoreUpdateGUI( 0 )
	, _activatedGroup( CallbackGroup::NONE )
{
}


void AttrViewer::Initialize( Panel_Attributes* panel, QTreeWidget* panelTree )
{
	_panel = panel;
	_panelTree = panelTree;
	_OnInitialize();
}


void AttrViewer::ChangeTarget( void* target )
{
	// reset group
	_activatedGroup = CallbackGroup::NONE;
	// change target
	_OnChangeTarget( target );
	// refresh panel
	RefreshPanel();
}


void AttrViewer::UpdateGUI()
{
	if ( _ignoreUpdateGUI > 0 )
	{
		--_ignoreUpdateGUI;
		return;
	}

	_UpdateGUI();
}


void AttrViewer::Clear()
{
	_attributes.Clear();
	_groups.Clear();
}


void AttrViewer::RefreshPanel()
{
	_panel->RefreshPanel();
}


void AttrViewer::ShowViewer()
{
	_titleItem->setHidden( FALSE );

	// recalculate height
	for ( int i = 0; i < _attributes.Size(); ++i ) {
		_attributes[i].treeWidget->A_CalculateHeight();
	}
}


void AttrViewer::A_OnChange_Group1()
{
	_activatedGroup = CallbackGroup::GROUP_1;
	_UpdateTarget();
}


void AttrViewer::A_OnChange_Group2()
{
	_activatedGroup = CallbackGroup::GROUP_2;
	_UpdateTarget();
}


void AttrViewer::QtSelector::A_OnOpenSelector()
{
	EditorApp::Singleton()->ShowSelectorPopup( this->type.GetChar(), this );
}


void AttrViewer::QtSelector::OnChangeSelection( void* ref, cchar* name )
{
	this->selected = ref;

	QLineEdit* textWidget = CAST_S( QLineEdit*, this->layout()->itemAt( 0 )->widget() );
	textWidget->setText( name );

	switch ( this->callbackGroup ) {
		case CallbackGroup::GROUP_1:	this->attrViewer->A_OnChange_Group1();	break;
		case CallbackGroup::GROUP_2:	this->attrViewer->A_OnChange_Group2();	break;
	}
}


void AttrViewer::_AddToPanel( cchar* title )
{
	_titleItem = Memory::Alloc<QTreeWidgetItem>();
	_titleItem->setFlags( Qt::ItemIsEnabled );
	_panelTree->addTopLevelItem( _titleItem );
	_panelTree->expandItem( _titleItem );

	QTreeWidgetItem* childItem = Memory::Alloc<QTreeWidgetItem>();
	childItem->setFlags( Qt::ItemIsEnabled );
	_titleItem->addChild( childItem );

	// add title
	{
		QHBoxLayout* boxLayout = Memory::Alloc<QHBoxLayout>();
		boxLayout->setAlignment( Qt::AlignLeft );

		if ( _innerAttributes.Has( InnerAttrType::ENABLED ) ) {
			boxLayout->addWidget( _innerAttributes.Get( InnerAttrType::ENABLED ).valueWidget );
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

		_panelTree->setItemWidget( childItem, 0, _childWidget );
	}

	_UpdatePanel();
}


void AttrViewer::_UpdatePanel()
{
	// add attributes

	// get layout, used to display both attributes and groups
	QLayout* childLayout = _childWidget->layout();

	int attrIndex = 0;
	int groupIndex = 0;

	// loop until all attributes are displayed
	while ( attrIndex < _attributes.Size() ) {
		CustomTreeWidget* treeWidget = Memory::Alloc<CustomTreeWidget>();
		treeWidget->setHeaderHidden( TRUE );
		treeWidget->setDragEnabled( FALSE );
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


		int first = attrIndex;
		int last = _attributes.Size() - 1;

		if ( groupIndex < _groups.Size() ) {
			_Group currGroup = _groups[groupIndex];
			int firstIndex = currGroup.firstIndex;

			if ( firstIndex == attrIndex ) {
				first = currGroup.firstIndex;
				last = first + currGroup.count - 1;
				++groupIndex;

				groupWidget->setTitle( currGroup.name.GetChar() );
			}
			else {
				last = firstIndex - 1;
			}
		}

		while ( first <= attrIndex && attrIndex <= last ) {
			QTreeWidgetItem* item = Memory::Alloc<QTreeWidgetItem>();
			item->setFlags( Qt::ItemIsEnabled );

			treeWidget->addTopLevelItem( item );
			item->setExpanded( TRUE );

			// set the attr name
			item->setText( 0, _attributes[attrIndex].label.GetChar() );

			// set the attr value
			treeWidget->setItemWidget( item, 1, _attributes[attrIndex].valueWidget );

			_attributes[attrIndex].treeItem = item;
			_attributes[attrIndex].treeWidget = treeWidget;

			if ( _attributes[attrIndex].type == AttrType::LIST ) {
				_ListWidget* listWidget = CAST_S( _ListWidget*, _attributes[attrIndex].valueWidget );
				listWidget->rootItem = item;
				listWidget->treeWidget = treeWidget;
				listWidget->attrViewer = this;
			}

			if ( _attributes[attrIndex].type == AttrType::STRUCT ) {
				_StructWidget* structWidget = CAST_S( _StructWidget*, _attributes[attrIndex].valueWidget );
				structWidget->rootItem = item;
				structWidget->treeWidget = treeWidget;

				DataArray<AttrStruct>& scheme = structWidget->structScheme;

				for ( int i = 0; i < scheme.Size(); ++i ) {
					QTreeWidgetItem* structItem = Memory::Alloc<QTreeWidgetItem>();
					structItem->setFlags( Qt::ItemIsEnabled );
					structItem->setText( 0, scheme[i].attrName.GetChar() );

					item->addChild( structItem );

					QWidget* valueWidget = NULL;
					switch ( scheme[i].attrType ) {
						case AttrType::STRING:
							valueWidget = CreateString( structWidget->flagMask,
														structWidget->callbackGroup );
							break;

						case AttrType::BOOL:
							valueWidget = CreateBool( structWidget->flagMask,
														structWidget->callbackGroup );
							break;

						case AttrType::INT:
							valueWidget = CreateInt( structWidget->flagMask,
														structWidget->callbackGroup );
							break;

						case AttrType::FLOAT:
							valueWidget = CreateFloat( structWidget->flagMask,
														structWidget->callbackGroup );
							break;

						case AttrType::VECTOR2:
							valueWidget = CreateVector2( structWidget->flagMask,
															structWidget->callbackGroup );
							break;

						case AttrType::VECTOR3:
							valueWidget = CreateVector3( structWidget->flagMask,
															structWidget->callbackGroup );
							break;

						case AttrType::VECTOR4:
							valueWidget = CreateVector4( structWidget->flagMask,
															structWidget->callbackGroup );
							break;

						case AttrType::DROPDOWN:
							valueWidget = CreateDropdown( scheme[i].dropdownValues,
															structWidget->flagMask,
															structWidget->callbackGroup );
							break;

						case AttrType::SELECTOR:
							valueWidget = CreateSelector( scheme[i].selectorDataType.GetChar(),
															structWidget->flagMask,
															structWidget->callbackGroup );
							break;

						case AttrType::LIST:
						{
							switch ( scheme[i].listType ) {
								case AttrType::STRING:
								case AttrType::BOOL:
								case AttrType::INT:
								case AttrType::FLOAT:
								case AttrType::VECTOR2:
								case AttrType::VECTOR3:
								case AttrType::VECTOR4:
									valueWidget = _CreateList( scheme[i].listType,
																structWidget->flagMask,
																structWidget->callbackGroup );
									break;

								case AttrType::DROPDOWN:
									valueWidget = _CreateListDropdown( scheme[i].dropdownValues,
																		structWidget->flagMask,
																		structWidget->callbackGroup );
									break;

								case AttrType::SELECTOR:
									valueWidget = _CreateListSelector( scheme[i].selectorDataType.GetChar(),
																		structWidget->flagMask,
																		structWidget->callbackGroup );
									break;

								case AttrType::STRUCT:
									valueWidget = _CreateListStruct( scheme[i].structScheme,
																		structWidget->flagMask,
																		structWidget->callbackGroup );
									break;
							}

							_ListWidget* listWidget = CAST_S( _ListWidget*, valueWidget );

							listWidget->rootItem = structItem;
							listWidget->treeWidget = treeWidget;
							listWidget->attrViewer = this;

							break;
						}

					}

					treeWidget->setItemWidget( structItem, 1, valueWidget );
				}
			}

			++attrIndex;
		}

		treeWidget->A_CalculateHeight();
	}
}


void AttrViewer::_UpdateVisibility()
{
	for ( int i = 0; i < _attributes.Size(); ++i )
	{
		_attributes[i].treeItem->setHidden( !_attributes[i].isVisible );
		_attributes[i].treeWidget->A_CalculateHeight();
	}

	RefreshPanel();
}


void AttrViewer::_ResetViewer()
{
	_groups.Clear();
	_attributes.Clear();

	// delete attribute widgets
	QtUtils::ClearLayout( _childWidget->layout() );
}


void AttrViewer::_OpenGroup( cchar* name )
{
	_groups.Add( _Group{ name, _attributes.Size(), 0 } );
}


void AttrViewer::_CloseGroup()
{
	int size = _groups.Size();
	ASSERT( size > 0 );

	_groups[ size - 1 ].count = _attributes.Size() - _groups[ size - 1 ].firstIndex;
}


void AttrViewer::_CreateAttrLabel( cchar* name, cchar* label )
{
	_SetAttribute( name, label, NULL, AttrType::NONE );
}


void AttrViewer::_CreateAttrString( cchar* name, cchar* label )
{
	_CreateAttrString( name, label, AttrFlag::NONE, CallbackGroup::GROUP_1 );
}


void AttrViewer::_CreateAttrBool( cchar* name, cchar* label )
{
	_CreateAttrBool( name, label, AttrFlag::NONE, CallbackGroup::GROUP_1 );
}


void AttrViewer::_CreateAttrInt( cchar* name, cchar* label )
{
	_CreateAttrInt( name, label, AttrFlag::NONE, CallbackGroup::GROUP_1 );
}


void AttrViewer::_CreateAttrFloat( cchar* name, cchar* label )
{
	_CreateAttrFloat( name, label, AttrFlag::NONE, CallbackGroup::GROUP_1 );
}


void AttrViewer::_CreateAttrVector2( cchar* name, cchar* label )
{
	_CreateAttrVector2( name, label, AttrFlag::NONE, CallbackGroup::GROUP_1 );
}


void AttrViewer::_CreateAttrVector3( cchar* name, cchar* label )
{
	_CreateAttrVector3( name, label, AttrFlag::NONE, CallbackGroup::GROUP_1 );
}


void AttrViewer::_CreateAttrVector4( cchar* name, cchar* label )
{
	_CreateAttrVector4( name, label, AttrFlag::NONE, CallbackGroup::GROUP_1 );
}


void AttrViewer::_CreateAttrDropdown( cchar* name, cchar* label, 
									  DataArray<cchar*>& valueList )
{
	_CreateAttrDropdown( name, label, valueList, AttrFlag::NONE, CallbackGroup::GROUP_1 );
}


void AttrViewer::_CreateAttrSelector( cchar* name, cchar* label, 
									  cchar* dataType )
{
	_CreateAttrSelector( name, label, dataType, AttrFlag::NONE, CallbackGroup::GROUP_1 );
}


void AttrViewer::_CreateAttrStruct( cchar* name, cchar* label, 
									DataArray<AttrStruct>& structScheme )
{
	_CreateAttrStruct( name, label, structScheme, AttrFlag::NONE, CallbackGroup::GROUP_1 );
}


void AttrViewer::_CreateAttrList( cchar* name, cchar* label, 
								  AttrType elementType )
{
	_CreateAttrList( name, label, elementType, AttrFlag::NONE, CallbackGroup::GROUP_1 );
}


void AttrViewer::_CreateAttrListSelector( cchar* name, cchar* label, 
										  cchar* dataType )
{
	_CreateAttrListSelector( name, label, dataType, AttrFlag::NONE, CallbackGroup::GROUP_1 );
}


void AttrViewer::_CreateAttrListDropdown( cchar* name, cchar* label, 
										  DataArray<cchar*>& valueList )
{
	_CreateAttrListDropdown( name, label, valueList, AttrFlag::NONE, CallbackGroup::GROUP_1 );
}


void AttrViewer::_CreateAttrListStruct( cchar* name, cchar* label, 
										DataArray<AttrStruct>& structScheme )
{
	_CreateAttrListStruct( name, label, structScheme, AttrFlag::NONE, CallbackGroup::GROUP_1 );
}


void AttrViewer::_CreateAttrString( cchar* name, cchar* label, 
									int flagMask, CallbackGroup group )
{
	QWidget* widget = CreateString( flagMask, group );
	_SetAttribute( name, label, widget, AttrType::STRING );
}


void AttrViewer::_CreateAttrBool( cchar* name, cchar* label, 
								  int flagMask, CallbackGroup group )
{
	QWidget* widget = CreateBool( flagMask, group );
	_SetAttribute( name, label, widget, AttrType::BOOL );
}


void AttrViewer::_CreateAttrInt( cchar* name, cchar* label, 
								 int flagMask, CallbackGroup group )
{
	QWidget* widget = CreateInt( flagMask, group );
	_SetAttribute( name, label, widget, AttrType::INT );
}


void AttrViewer::_CreateAttrFloat( cchar* name, cchar* label, 
								   int flagMask, CallbackGroup group )
{
	QWidget* widget = CreateFloat( flagMask, group );
	_SetAttribute( name, label, widget, AttrType::FLOAT );
}


void AttrViewer::_CreateAttrVector2( cchar* name, cchar* label, 
									 int flagMask, CallbackGroup group )
{
	QWidget* widget = CreateVector2( flagMask, group );
	_SetAttribute( name, label, widget, AttrType::VECTOR2 );
}


void AttrViewer::_CreateAttrVector3( cchar* name, cchar* label, 
									 int flagMask, CallbackGroup group )
{
	QWidget* widget = CreateVector3( flagMask, group );
	_SetAttribute( name, label, widget, AttrType::VECTOR3 );
}


void AttrViewer::_CreateAttrVector4( cchar* name, cchar* label, 
									 int flagMask, CallbackGroup group )
{
	QWidget* widget = CreateVector4( flagMask, group );
	_SetAttribute( name, label, widget, AttrType::VECTOR4 );
}


void AttrViewer::_CreateAttrDropdown( cchar* name, cchar* label, 
									  DataArray<cchar*>& valueList, 
									  int flagMask, CallbackGroup group )
{
	QWidget* widget = CreateDropdown( valueList, flagMask, group );
	_SetAttribute( name, label, widget, AttrType::DROPDOWN );
}


void AttrViewer::_CreateAttrSelector( cchar* name, cchar* label, cchar* dataType, 
									  int flagMask, CallbackGroup group )
{
	QWidget* widget = CreateSelector( dataType, flagMask, group );
	_SetAttribute( name, label, widget, AttrType::SELECTOR );
}


void AttrViewer::_CreateAttrStruct( cchar* name, cchar* label, 
									DataArray<AttrStruct>& structScheme, 
									int flagMask, CallbackGroup group )
{
	for ( int i = 0; i < structScheme.Size(); ++i )
	{
		ASSERT( structScheme[i].attrType != AttrType::STRUCT );
		ASSERT( structScheme[i].attrName != NULL );
	}

	QWidget* widget = CreateStruct( structScheme, flagMask, group );
	_SetAttribute( name, label, widget, AttrType::STRUCT );
}


void AttrViewer::_CreateAttrList( cchar* name, cchar* label, AttrType elementType, 
								  int flagMask, CallbackGroup group )
{
	ASSERT( elementType != AttrType::SELECTOR );
	ASSERT( elementType != AttrType::LIST );
	ASSERT( elementType != AttrType::DROPDOWN );
	ASSERT( elementType != AttrType::STRUCT );

	QWidget* widget = _CreateList( elementType, flagMask, group );
	_SetAttribute( name, label, widget, AttrType::LIST );
}


void AttrViewer::_CreateAttrListSelector( cchar* name, cchar* label, 
										  cchar* dataType, 
										  int flagMask, CallbackGroup group )
{
	QWidget* widget = _CreateListSelector( dataType, flagMask, group );
	_SetAttribute( name, label, widget, AttrType::LIST );
}


void AttrViewer::_CreateAttrListDropdown( cchar* name, cchar* label, 
										  DataArray<cchar*>& valueList, 
										  int flagMask, CallbackGroup group )
{
	QWidget* widget = _CreateListDropdown( valueList, flagMask, group );
	_SetAttribute( name, label, widget, AttrType::LIST );
}


void AttrViewer::_CreateAttrListStruct( cchar* name, cchar* label, 
										DataArray<AttrStruct>& structScheme, 
										int flagMask, CallbackGroup group )
{
	QWidget* widget = _CreateListStruct( structScheme, flagMask, group );
	_SetAttribute( name, label, widget, AttrType::LIST );
}


void AttrViewer::_WriteAttrString( cchar* name, cchar* value )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::STRING );

	_WriteString( attr.valueWidget, value );
}


void AttrViewer::_WriteAttrFloat( cchar* name, float value )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::FLOAT );

	_WriteFloat( attr.valueWidget, value );
}


void AttrViewer::_WriteAttrInt( cchar* name, int value )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::INT );

	_WriteInt( attr.valueWidget, value );
}


void AttrViewer::_WriteAttrBool( cchar* name, bool value )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::BOOL );

	_WriteBool( attr.valueWidget, value );
}


void AttrViewer::_WriteAttrVector2( cchar* name, const Vector2& value )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::VECTOR2 );

	_WriteVector2( attr.valueWidget, value );
}


void AttrViewer::_WriteAttrVector3( cchar* name, const Vector3& value )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::VECTOR3 );

	_WriteVector3( attr.valueWidget, value );
}


void AttrViewer::_WriteAttrVector4( cchar* name, const Vector4& value )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::VECTOR4 );

	_WriteVector4( attr.valueWidget, value );
}


void AttrViewer::_WriteAttrDropdown( cchar* name, int valueIndex )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::DROPDOWN );

	_WriteDropdown( attr.valueWidget, valueIndex );
}


void AttrViewer::_WriteAttrSelector( cchar* name, void* valueRef, cchar* valueName )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::SELECTOR );

	_WriteSelector( attr.valueWidget, valueRef, valueName );
}


void AttrViewer::_WriteAttrStruct( cchar* name, cchar* elemName, 
								   const DataUnion& elemValue, cchar* valueName )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::STRUCT );

	_StructWidget* structWidget = CAST_S( _StructWidget*, attr.valueWidget );

	int structIndex = -1;
	QWidget* valueWidget = _FindStructElem( structWidget, elemName, structIndex );

	switch ( structWidget->structScheme[structIndex].attrType )
	{
		case AttrType::STRING:
			_WriteString( valueWidget, elemValue.GetString() );
			break;

		case AttrType::BOOL:
			_WriteBool( valueWidget, elemValue.GetBool() );
			break;

		case AttrType::FLOAT:
			_WriteFloat( valueWidget, elemValue.GetFloat() );
			break;

		case AttrType::INT:
			_WriteInt( valueWidget, elemValue.GetInt() );
			break;

		case AttrType::VECTOR2:
			_WriteVector2( valueWidget, elemValue.GetVector2() );
			break;

		case AttrType::VECTOR3:
			_WriteVector3( valueWidget, elemValue.GetVector3() );
			break;

		case AttrType::VECTOR4:
			_WriteVector4( valueWidget, elemValue.GetVector4() );
			break;

		case AttrType::DROPDOWN:
			_WriteDropdown( valueWidget, elemValue.GetInt() );
			break;

		case AttrType::SELECTOR:
			_WriteSelector( valueWidget, elemValue.GetReference(), valueName );
			break;
	}
}


void AttrViewer::_WriteAttrListSize( cchar* name, int size )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::LIST );

	_ListWidget* listWidget = CAST_S( _ListWidget*, attr.valueWidget );

	QLineEdit* sizeWidget = CAST_S( QLineEdit*, listWidget->layout()->itemAt(1)->widget() );
	sizeWidget->blockSignals( TRUE );
	sizeWidget->setText( QString::number( size ) );
	sizeWidget->blockSignals( FALSE );

	listWidget->A_OnSizeChanged();
}


void AttrViewer::_WriteAttrListIndex( cchar* name, int index, DataUnion& elemValue,
									  cchar* valueName )
{
	_WriteAttrListIndex( name, index, NULL, elemValue, valueName );
}


void AttrViewer::_WriteAttrListIndex( cchar* name, int index, cchar* elemName, 
									  DataUnion& elemValue, cchar* valueName )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::LIST );

	_WriteListIndex( attr.valueWidget, index, elemName, elemValue, valueName );
}


void AttrViewer::_WriteAttrStrListSize( cchar* name, cchar* listName, int size )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::STRUCT );

	_StructWidget* structWidget = CAST_S( _StructWidget*, attr.valueWidget );

	int structIndex = -1;
	QWidget* valueWidget = _FindStructElem( structWidget, listName, structIndex );

	ASSERT( structWidget->structScheme[structIndex].attrType == AttrType::LIST );
	
	_ListWidget* listWidget = CAST_S( _ListWidget*, valueWidget );

	QLineEdit* sizeWidget = CAST_S( QLineEdit*, listWidget->layout()->itemAt(1)->widget() );
	sizeWidget->blockSignals( TRUE );
	sizeWidget->setText( QString::number( size ) );
	sizeWidget->blockSignals( FALSE );

	listWidget->A_OnSizeChanged();
}


void AttrViewer::_WriteAttrStrListIndex( cchar* name, cchar* listName, int index, 
										 DataUnion& elemValue, cchar* valueName )
{
	_WriteAttrStrListIndex( name, listName, index, NULL, elemValue, valueName );
}


void AttrViewer::_WriteAttrStrListIndex( cchar* name, cchar* listName, int index, 
										 cchar* elemName, DataUnion& elemValue, 
										 cchar* valueName )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::STRUCT );

	_StructWidget* structWidget = CAST_S( _StructWidget*, attr.valueWidget );

	int structIndex = -1;
	QWidget* valueWidget = _FindStructElem( structWidget, listName, structIndex );

	ASSERT( structWidget->structScheme[structIndex].attrType == AttrType::LIST );
	
	_ListWidget* listWidget = CAST_S( _ListWidget*, valueWidget );
	
	_WriteListIndex( listWidget, index, elemName, elemValue, valueName );
}


String AttrViewer::_ReadAttrString( cchar* name )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::STRING );

	return _ReadString( attr.valueWidget );
}


float AttrViewer::_ReadAttrFloat( cchar* name )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::FLOAT );

	return _ReadFloat( attr.valueWidget );
}


int AttrViewer::_ReadAttrInt( cchar* name )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::INT );

	return _ReadInt( attr.valueWidget );
}


bool AttrViewer::_ReadAttrBool( cchar* name )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::BOOL );

	return _ReadBool( attr.valueWidget );
}


Vector2 AttrViewer::_ReadAttrVector2( cchar* name )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::VECTOR2 );

	return _ReadVector2( attr.valueWidget );
}


Vector3 AttrViewer::_ReadAttrVector3( cchar* name )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::VECTOR3 );

	return _ReadVector3( attr.valueWidget );
}


Vector4 AttrViewer::_ReadAttrVector4( cchar* name )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::VECTOR4 );

	return _ReadVector4( attr.valueWidget );
}


int AttrViewer::_ReadAttrDropdown( cchar* name )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::DROPDOWN );

	return _ReadDropdown( attr.valueWidget );
}


void* AttrViewer::_ReadAttrSelector( cchar* name )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::SELECTOR );

	return _ReadSelector( attr.valueWidget );
}


DataUnion AttrViewer::_ReadAttrStruct( cchar* name, cchar* elemName )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::STRUCT );

	_StructWidget* structWidget = CAST_S( _StructWidget*, attr.valueWidget );

	int structIndex = -1;
	QWidget* valueWidget = _FindStructElem( structWidget, elemName, structIndex );
	DataUnion attrValue;

	switch ( structWidget->structScheme[structIndex].attrType )
	{
		case AttrType::STRING:
			attrValue.SetString( _ReadString( valueWidget ).GetChar() );
			break;

		case AttrType::BOOL:
			attrValue.SetBool( _ReadBool( valueWidget ) );
			break;

		case AttrType::FLOAT:
			attrValue.SetFloat( _ReadFloat( valueWidget ) );
			break;

		case AttrType::INT:
			attrValue.SetInt( _ReadInt( valueWidget ) );
			break;

		case AttrType::VECTOR2:
			attrValue.SetVector2( _ReadVector2( valueWidget ) );
			break;

		case AttrType::VECTOR3:
			attrValue.SetVector3( _ReadVector3( valueWidget ) );
			break;

		case AttrType::VECTOR4:
			attrValue.SetVector4( _ReadVector4( valueWidget ) );
			break;

		case AttrType::DROPDOWN:
			attrValue.SetInt( _ReadDropdown( valueWidget ) );
			break;

		case AttrType::SELECTOR:
			attrValue.SetReference( _ReadSelector( valueWidget ) );
			break;
	}

	return attrValue;
}


int AttrViewer::_ReadAttrListSize( cchar* name )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::LIST );

	_ListWidget* widget = CAST_S( _ListWidget*, attr.valueWidget );

	QLineEdit* sizeWidget = CAST_S( QLineEdit*, widget->layout()->itemAt(1)->widget() );

	return sizeWidget->text().toInt();
}


DataUnion AttrViewer::_ReadAttrListIndex( cchar* name, int index )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::LIST );

	return _ReadListIndex( attr.valueWidget, index, NULL );
}


DataUnion AttrViewer::_ReadAttrListIndex( cchar* name, int index, cchar* elemName )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::LIST );

	return _ReadListIndex( attr.valueWidget, index, elemName );
}


int AttrViewer::_ReadAttrStrListSize( cchar* name, cchar* listName )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::STRUCT );

	_StructWidget* structWidget = CAST_S( _StructWidget*, attr.valueWidget );

	int structIndex = -1;
	QWidget* valueWidget = _FindStructElem( structWidget, listName, structIndex );

	ASSERT( structWidget->structScheme[structIndex].attrType == AttrType::LIST );
	
	_ListWidget* listWidget = CAST_S( _ListWidget*, valueWidget );

	QLineEdit* sizeWidget = CAST_S( QLineEdit*, listWidget->layout()->itemAt(1)->widget() );

	return sizeWidget->text().toInt();
}


DataUnion AttrViewer::_ReadAttrStrListIndex( cchar* name, cchar* listName, int index )
{
	return _ReadAttrStrListIndex( name, listName, index, NULL );
}


DataUnion AttrViewer::_ReadAttrStrListIndex( cchar* name, cchar* listName, int index, 
											 cchar* elemName )
{
	_Attribute attr;
	bool doesExist = _GetAttribute( name, attr );
	ASSERT( doesExist );
	ASSERT( attr.type == AttrType::STRUCT );

	_StructWidget* structWidget = CAST_S( _StructWidget*, attr.valueWidget );

	int structIndex = -1;
	QWidget* valueWidget = _FindStructElem( structWidget, listName, structIndex );

	ASSERT( structWidget->structScheme[structIndex].attrType == AttrType::LIST );
	
	_ListWidget* listWidget = CAST_S( _ListWidget*, valueWidget );

	return _ReadListIndex( listWidget, index, elemName );
}


void AttrViewer::_CreateInnerAttribute( InnerAttrType innerType )
{
	_InnerAttribute attr;

	switch ( innerType )
	{
		case InnerAttrType::ENABLED:
			attr.type = AttrType::BOOL;
			attr.valueWidget = CreateBool( AttrFlag::NONE, CallbackGroup::GROUP_1 );
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


void AttrViewer::_SetAttrVisibility( cchar* name, bool value )
{
	for ( int i = 0; i < _attributes.Size(); ++i )
	{
		if ( _attributes[i].name == name )
		{
			_attributes[i].isVisible = value;
			return;
		}
	}

	ASSERT( FALSE );
}


void AttrViewer::_Colorize( bool enabled, bool colorizeAll )
{
	ASSERT( _panelTree != NULL );

	if ( colorizeAll ) {
		_panelTree->setProperty( EditorSkin::FLAG_DISABLED, !enabled );
	}
	else {
		_titleWidget->setProperty( EditorSkin::FLAG_DISABLED, !enabled );
		_childWidget->setProperty( EditorSkin::FLAG_DISABLED, !enabled );
	}
	RefreshPanel();
}


void AttrViewer::_SetAttribute( cchar* name, cchar* label, QWidget* widget, AttrType type )
{
	for ( int i = 0; i < _attributes.Size(); ++i )
	{
		if ( _attributes[i].name == name )
		{
			_attributes[i].valueWidget = widget;
			_attributes[i].type = type;
			_attributes[i].label = label;
			return;
		}
	}

	//! is a new element
	_attributes.Add( _Attribute { name, label, NULL, NULL, widget, type, TRUE } );
}


bool AttrViewer::_GetAttribute( cchar* name, OUT _Attribute& attribute)
{
	for ( int i = 0; i < _attributes.Size(); ++i )
	{
		if ( _attributes[i].name == name )
		{
			attribute = _attributes[i];
			return TRUE;
		}
	}

	return FALSE;
}


QWidget* AttrViewer::CreateString( int flagMask, CallbackGroup group )
{
	QLineEdit* widget = Memory::Alloc<QLineEdit>();
	widget->setReadOnly( (flagMask & AttrFlag::READONLY) != 0 );

	switch ( group )
	{
		case CallbackGroup::GROUP_1:
			if ( (flagMask & AttrFlag::EDIT_FINISH) != 0 ) {
				QObject::connect( widget, &QLineEdit::editingFinished, 
								  this, &AttrViewer::A_OnChange_Group1 );
			}
			else {
				QObject::connect( widget, &QLineEdit::textEdited, 
								  this, &AttrViewer::A_OnChange_Group1 );
			}
			break;

		case CallbackGroup::GROUP_2:
			if ( (flagMask & AttrFlag::EDIT_FINISH) != 0 ) {
				QObject::connect( widget, &QLineEdit::editingFinished, 
								  this, &AttrViewer::A_OnChange_Group2 );
			}
			else {
				QObject::connect( widget, &QLineEdit::textEdited, 
								  this, &AttrViewer::A_OnChange_Group2 );
			}
			break;
	}

	return widget;
}


QWidget* AttrViewer::CreateFloat( int flagMask, CallbackGroup group )
{
	QLineEdit* widget = Memory::Alloc<QLineEdit>();
	widget->setValidator( Memory::Alloc<QDoubleValidator>( widget ) );
	widget->setReadOnly( (flagMask & AttrFlag::READONLY) != 0 );
	widget->setText( "0" );

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


QWidget* AttrViewer::CreateInt( int flagMask, CallbackGroup group )
{
	QLineEdit* widget = Memory::Alloc<QLineEdit>();
	widget->setValidator( Memory::Alloc<QIntValidator>( widget ) );
	widget->setReadOnly( (flagMask & AttrFlag::READONLY) != 0 );
	widget->setText( "0" );

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


QWidget* AttrViewer::CreateBool( int flagMask, CallbackGroup group )
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


QWidget* AttrViewer::CreateVector2( int flagMask, CallbackGroup group )
{
	QHBoxLayout* boxLayout = Memory::Alloc<QHBoxLayout>();
	boxLayout->setAlignment( Qt::AlignLeft );
	boxLayout->setSpacing( 0 );
	boxLayout->setContentsMargins( 0, 0, 0, 0 );

	boxLayout->addWidget( Memory::Alloc<QLabel>( " X" ) );
	boxLayout->addWidget( CreateFloat( flagMask, group ) );
	boxLayout->addWidget( Memory::Alloc<QLabel>( " Y" ) );
	boxLayout->addWidget( CreateFloat( flagMask, group ) );

	QWidget* widget = Memory::Alloc<QWidget>();
	widget->setLayout( boxLayout );

	return widget;
}


QWidget* AttrViewer::CreateVector3( int flagMask, CallbackGroup group )
{
	QHBoxLayout* boxLayout = Memory::Alloc<QHBoxLayout>();
	boxLayout->setAlignment( Qt::AlignLeft );
	boxLayout->setSpacing( 0 );
	boxLayout->setContentsMargins( 0, 0, 0, 0 );

	boxLayout->addWidget( Memory::Alloc<QLabel>( " X" ) );
	boxLayout->addWidget( CreateFloat( flagMask, group ) );
	boxLayout->addWidget( Memory::Alloc<QLabel>( " Y" ) );
	boxLayout->addWidget( CreateFloat( flagMask, group ) );
	boxLayout->addWidget( Memory::Alloc<QLabel>( " Z" ) );
	boxLayout->addWidget( CreateFloat( flagMask, group ) );


	QWidget* widget = Memory::Alloc<QWidget>();
	widget->setLayout( boxLayout );

	return widget;
}


QWidget* AttrViewer::CreateVector4( int flagMask, CallbackGroup group )
{
	QHBoxLayout* boxLayout = Memory::Alloc<QHBoxLayout>();
	boxLayout->setAlignment( Qt::AlignLeft );
	boxLayout->setSpacing( 0 );
	boxLayout->setContentsMargins( 0, 0, 0, 0 );

	boxLayout->addWidget( Memory::Alloc<QLabel>( " X" ) );
	boxLayout->addWidget( CreateFloat( flagMask, group ) );
	boxLayout->addWidget( Memory::Alloc<QLabel>( " Y" ) );
	boxLayout->addWidget( CreateFloat( flagMask, group ) );
	boxLayout->addWidget( Memory::Alloc<QLabel>( " Z" ) );
	boxLayout->addWidget( CreateFloat( flagMask, group ) );
	boxLayout->addWidget( Memory::Alloc<QLabel>( " W" ) );
	boxLayout->addWidget( CreateFloat( flagMask, group ) );

	QWidget* widget = Memory::Alloc<QWidget>();
	widget->setLayout( boxLayout );

	return widget;
}


QWidget* AttrViewer::CreateDropdown( DataArray<cchar*>& valueList, int flagMask,
									  CallbackGroup group )
{
	QStringList stringList;
	for ( int i = 0; i < valueList.Size(); ++i )
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


QWidget* AttrViewer::CreateSelector( cchar* dataType, int flagMask, CallbackGroup group )
{
	QHBoxLayout* boxLayout = Memory::Alloc<QHBoxLayout>();
	boxLayout->setAlignment( Qt::AlignLeft );
	boxLayout->setSpacing( 0 );
	boxLayout->setContentsMargins( 0, 0, 0, 0 );

	QLineEdit* textWidget = Memory::Alloc<QLineEdit>();
	textWidget->setReadOnly( TRUE );

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


QWidget* AttrViewer::CreateStruct( DataArray<AttrStruct>& structScheme, 
								   int flagMask, CallbackGroup group )
{
	QHBoxLayout* boxLayout = Memory::Alloc<QHBoxLayout>();
	boxLayout->setAlignment( Qt::AlignLeft );
	boxLayout->setSpacing( 0 );
	boxLayout->setContentsMargins( 0, 0, 0, 0 );

	FiniteString structDisplay( "{ %d }", structScheme.Size() );

	boxLayout->addWidget( Memory::Alloc<QLabel>( structDisplay.GetChar() ) );

	_StructWidget* widget = Memory::Alloc<_StructWidget>();
	widget->setLayout( boxLayout );
	widget->structScheme = structScheme;
	widget->flagMask = flagMask;
	widget->callbackGroup = group;

	return widget;
}


QWidget* AttrViewer::_CreateList( AttrType elementType, int flagMask, CallbackGroup group )
{
	QLineEdit* lineEdit = Memory::Alloc<QLineEdit>();
	lineEdit->setValidator( Memory::Alloc<QIntValidator>( lineEdit ) );
	lineEdit->setReadOnly( (flagMask & AttrFlag::READONLY) != 0 );
	lineEdit->setText( "0" );

	QHBoxLayout* boxLayout = Memory::Alloc<QHBoxLayout>();
	boxLayout->setAlignment( Qt::AlignLeft );
	boxLayout->setSpacing( 0 );
	boxLayout->setContentsMargins( 0, 0, 0, 0 );

	boxLayout->addWidget( Memory::Alloc<QLabel>( "[..]" ) );
	boxLayout->addWidget( lineEdit );

	_ListWidget* widget = Memory::Alloc<_ListWidget>();
	widget->setLayout( boxLayout );
	widget->elementType = elementType;
	widget->callbackGroup = group;
	widget->flagMask = flagMask;

	QObject::connect( lineEdit, &QLineEdit::textEdited, widget, &_ListWidget::A_OnSizeChanged );

	switch ( group )
	{
		case CallbackGroup::GROUP_1:
			QObject::connect( lineEdit, &QLineEdit::textEdited, this, &AttrViewer::A_OnChange_Group1 );
			break;

		case CallbackGroup::GROUP_2:
			QObject::connect( lineEdit, &QLineEdit::textEdited, this, &AttrViewer::A_OnChange_Group2 );
			break;
	}

	return widget;
}


QWidget* AttrViewer::_CreateListDropdown( DataArray<cchar*>& valueList, int flagMask, 
										  CallbackGroup group )
{
	QWidget* widget = _CreateList( AttrType::DROPDOWN, flagMask, group );

	_ListWidget* listWidget = CAST_S( _ListWidget*, widget );
	listWidget->dropdownValues = valueList;

	return widget;
}


QWidget* AttrViewer::_CreateListSelector( cchar* dataType, int flagMask, 
										  CallbackGroup group )
{
	QWidget* widget = _CreateList( AttrType::SELECTOR, flagMask, group );

	_ListWidget* listWidget = CAST_S( _ListWidget*, widget );
	listWidget->selectorDataType = dataType;

	return widget;
}


QWidget* AttrViewer::_CreateListStruct( DataArray<AttrStruct>& structScheme, int flagMask, 
										CallbackGroup group )
{
	QWidget* widget = _CreateList( AttrType::STRUCT, flagMask, group );

	_ListWidget* listWidget = CAST_S( _ListWidget*, widget );
	listWidget->structScheme = structScheme;

	return widget;
}


QWidget* AttrViewer::_FindStructElem( _StructWidget* structWidget, cchar* elemName, 
									  OUT int& elemIndex )
{
	QTreeWidgetItem* rootItem = structWidget->rootItem;
	QTreeWidgetItem* structItem = NULL;

	for ( int i = 0; i < rootItem->childCount(); ++i )
	{
		if ( rootItem->child( i )->text( 0 ).compare( elemName ) == 0 )
		{
			structItem = rootItem->child( i );
			elemIndex = i;
			break;
		}
	}

	ASSERT( structItem != NULL );

	return structWidget->treeWidget->itemWidget( structItem, 1 );;
}


void AttrViewer::_WriteString( QWidget* widget, cchar* value )
{
	QLineEdit* textWidget = CAST_S( QLineEdit*, widget );
	textWidget->blockSignals( TRUE );
	textWidget->setText( value );
	textWidget->blockSignals( FALSE );
}


void AttrViewer::_WriteFloat( QWidget* widget, float value )
{
	QLineEdit* textWidget = CAST_S( QLineEdit*, widget );
	textWidget->blockSignals( TRUE );
	textWidget->setText( QString::number( value ) );
	textWidget->blockSignals( FALSE );
}


void AttrViewer::_WriteInt( QWidget* widget, int value )
{
	QLineEdit* textWidget = CAST_S( QLineEdit*, widget );
	textWidget->blockSignals( TRUE );
	textWidget->setText( QString::number( value ) );
	textWidget->blockSignals( FALSE );
}


void AttrViewer::_WriteBool( QWidget* widget, bool value )
{
	CAST_S( QCheckBox*, widget )->setChecked( value );
}


void AttrViewer::_WriteVector2( QWidget* widget, const Vector2& value )
{
	QLineEdit* xWidget;
	QLineEdit* yWidget;

	xWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 1 )->widget() );
	xWidget->blockSignals( TRUE );
	xWidget->setText( QString::number( value.x ) );
	xWidget->blockSignals( FALSE );

	yWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 3 )->widget() );
	yWidget->blockSignals( TRUE );
	yWidget->setText( QString::number( value.y ) );
	yWidget->blockSignals( FALSE );
}


void AttrViewer::_WriteVector3( QWidget* widget, const Vector3& value )
{
	QLineEdit* xWidget;
	QLineEdit* yWidget;
	QLineEdit* zWidget;

	xWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 1 )->widget() );
	xWidget->blockSignals( TRUE );
	xWidget->setText( QString::number( value.x ) );
	xWidget->blockSignals( FALSE );

	yWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 3 )->widget() );
	yWidget->blockSignals( TRUE );
	yWidget->setText( QString::number( value.y ) );
	yWidget->blockSignals( FALSE );

	zWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 5 )->widget() );
	zWidget->blockSignals( TRUE );
	zWidget->setText( QString::number( value.z ) );
	zWidget->blockSignals( FALSE );
}


void AttrViewer::_WriteVector4( QWidget* widget, const Vector4& value )
{
	QLineEdit* xWidget;
	QLineEdit* yWidget;
	QLineEdit* zWidget;
	QLineEdit* wWidget;

	xWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 1 )->widget() );
	xWidget->blockSignals( TRUE );
	xWidget->setText( QString::number( value.x ) );
	xWidget->blockSignals( FALSE );

	yWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 3 )->widget() );
	yWidget->blockSignals( TRUE );
	yWidget->setText( QString::number( value.y ) );
	yWidget->blockSignals( FALSE );

	zWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 5 )->widget() );
	zWidget->blockSignals( TRUE );
	zWidget->setText( QString::number( value.z ) );
	zWidget->blockSignals( FALSE );

	wWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 7 )->widget() );
	wWidget->blockSignals( TRUE );
	wWidget->setText( QString::number( value.w ) );
	wWidget->blockSignals( FALSE );
}


void AttrViewer::_WriteDropdown( QWidget* widget, int valueIndex )
{
	QComboBox* comboBox = CAST_S( QComboBox*, widget );
	comboBox->blockSignals( TRUE );
	comboBox->setCurrentIndex( valueIndex );
	comboBox->blockSignals( FALSE );
}


void AttrViewer::_WriteSelector( QWidget* widget, void* valueRef, cchar* valueName )
{
	widget->blockSignals( TRUE );

	QtSelector* selector = CAST_S( QtSelector*, widget );
	selector->selected = valueRef;

	QLineEdit* textWidget = CAST_S( QLineEdit*, widget->layout()->itemAt( 0 )->widget() );
	textWidget->setText( valueName );

	widget->blockSignals( FALSE );
}


void AttrViewer::_WriteListIndex( QWidget* widget, int index, cchar* elemName, 
								  DataUnion& elemValue, cchar* valueName )
{
	_ListWidget* listWidget = CAST_S( _ListWidget*, widget );

	QLineEdit* sizeWidget = CAST_S( QLineEdit*, listWidget->layout()->itemAt(1)->widget() );
	int size = sizeWidget->text().toInt();

	ASSERT( index < size );

	QTreeWidgetItem* rootItem = listWidget->rootItem;
	QTreeWidgetItem* elemItem = rootItem->child( index );

	QWidget* valueWidget = NULL;
	AttrType valueType;

	if ( elemName != NULL )
	{
		QTreeWidgetItem* structItem = NULL;
		int structIndex = -1;

		for ( int i = 0; i < elemItem->childCount(); ++i )
		{
			if ( elemItem->child( i )->text( 0 ).compare( elemName ) == 0 )
			{
				structItem = elemItem->child( i );
				structIndex = i;
				break;
			}
		}

		ASSERT( structItem != NULL);

		valueWidget = listWidget->treeWidget->itemWidget( structItem, 1 );
		valueType = listWidget->structScheme[structIndex].attrType;
	}
	else
	{
		valueWidget = listWidget->treeWidget->itemWidget( elemItem, 1 );
		valueType = listWidget->elementType;
	}

	switch ( valueType )
	{
		case AttrType::STRING:
			_WriteString( valueWidget, elemValue.GetString() );
			break;

		case AttrType::BOOL:
			_WriteBool( valueWidget, elemValue.GetBool() );
			break;

		case AttrType::FLOAT:
			_WriteFloat( valueWidget, elemValue.GetFloat() );
			break;

		case AttrType::INT:
			_WriteInt( valueWidget, elemValue.GetInt() );
			break;

		case AttrType::VECTOR2:
			_WriteVector2( valueWidget, elemValue.GetVector2() );
			break;

		case AttrType::VECTOR3:
			_WriteVector3( valueWidget, elemValue.GetVector3() );
			break;

		case AttrType::VECTOR4:
			_WriteVector4( valueWidget, elemValue.GetVector4() );
			break;

		case AttrType::DROPDOWN:
			_WriteDropdown( valueWidget, elemValue.GetInt() );
			break;

		case AttrType::SELECTOR:
			_WriteSelector( valueWidget, elemValue.GetReference(), valueName );
			break;
	}
}


String AttrViewer::_ReadString( QWidget* widget )
{
	return CAST_S( QLineEdit*, widget )->text().toUtf8().constData();
}


float AttrViewer::_ReadFloat( QWidget* widget )
{
	return CAST_S( QLineEdit*, widget )->text().toFloat();
}


int AttrViewer::_ReadInt( QWidget* widget )
{
	return CAST_S( QLineEdit*, widget )->text().toInt();
}


bool AttrViewer::_ReadBool( QWidget* widget )
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


int AttrViewer::_ReadDropdown( QWidget* widget )
{
	return CAST_S( QComboBox*, widget )->currentIndex();
}


void* AttrViewer::_ReadSelector( QWidget* widget )
{
	return CAST_S( QtSelector*, widget )->selected;
}


DataUnion AttrViewer::_ReadListIndex( QWidget* widget, int index, cchar* elemName )
{
	_ListWidget* listWidget = CAST_S( _ListWidget*, widget );

	QLineEdit* sizeWidget = CAST_S( QLineEdit*, listWidget->layout()->itemAt(1)->widget() );
	int size = sizeWidget->text().toInt();

	ASSERT( index < size );

	QTreeWidgetItem* rootItem = listWidget->rootItem;
	QTreeWidgetItem* elemItem = rootItem->child( index );

	QWidget* valueWidget = NULL;
	AttrType valueType;

	if ( elemName != NULL )
	{
		QTreeWidgetItem* structItem = NULL;
		int structIndex = -1;

		for ( int i = 0; i < elemItem->childCount(); ++i )
		{
			if ( elemItem->child( i )->text( 0 ).compare( elemName ) == 0 )
			{
				structItem = elemItem->child( i );
				structIndex = i;
				break;
			}
		}

		ASSERT( structItem != NULL);

		valueWidget = listWidget->treeWidget->itemWidget( structItem, 1 );
		valueType = listWidget->structScheme[structIndex].attrType;
	}
	else
	{
		valueWidget = listWidget->treeWidget->itemWidget( elemItem, 1 );
		valueType = listWidget->elementType;
	}

	DataUnion attrValue;

	switch ( valueType )
	{
		case AttrType::STRING:
			attrValue.SetString( _ReadString( valueWidget ).GetChar() );
			break;

		case AttrType::BOOL:
			attrValue.SetBool( _ReadBool( valueWidget ) );
			break;

		case AttrType::FLOAT:
			attrValue.SetFloat( _ReadFloat( valueWidget ) );
			break;

		case AttrType::INT:
			attrValue.SetInt( _ReadInt( valueWidget ) );
			break;

		case AttrType::VECTOR2:
			attrValue.SetVector2( _ReadVector2( valueWidget ) );
			break;

		case AttrType::VECTOR3:
			attrValue.SetVector3( _ReadVector3( valueWidget ) );
			break;

		case AttrType::VECTOR4:
			attrValue.SetVector4( _ReadVector4( valueWidget ) );
			break;

		case AttrType::DROPDOWN:
			attrValue.SetInt( _ReadDropdown( valueWidget ) );
			break;

		case AttrType::SELECTOR:
			attrValue.SetReference( _ReadSelector( valueWidget ) );
			break;
	}

	return attrValue;
}


