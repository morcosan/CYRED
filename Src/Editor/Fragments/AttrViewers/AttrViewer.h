// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "CyredBuildingBlocks.h"

#include "QtCore\qobject.h"
#include "QtWidgets\qwidget.h"

class QTreeWidget;
class QLineEdit;
class QTreeWidgetItem;

namespace CYRED
{
	class CustomTreeWidget;
	class AttributePanel;
}


namespace CYRED
{
	namespace Enum_AttrType
	{
		enum Enum
		{
			NONE
			, STRING
			, FLOAT
			, INT
			, BOOL
			, VECTOR2
			, VECTOR3
			, VECTOR4
			, DROPDOWN
			, SELECTOR
			, LIST
			, STRUCT
		};
	}
	namespace Enum_AttrFlag
	{
		enum Enum
		{
			NONE			= 0x0
			, READONLY		= 0x1
			, EDIT_FINISH	= 0x2
		};
	}
	namespace Enum_CallbackGroup
	{
		enum Enum
		{
			GROUP_1
			, GROUP_2
		};
	}
	namespace Enum_InnerAttrType
	{
		enum Enum
		{
			ENABLED
		};
	}

	typedef Enum_AttrType::Enum			AttrType;
	typedef Enum_AttrFlag::Enum			AttrFlag;
	typedef Enum_CallbackGroup::Enum	CallbackGroup;
	typedef Enum_InnerAttrType::Enum	InnerAttrType;
}


namespace CYRED
{
	ABSTRACT class AttrViewer : public QObject
	{
	public:
		struct QtSelector : public QWidget
		{
			String			type;
			void*			selected;
			CallbackGroup	callbackGroup;
			AttrViewer*		attrViewer;

			void A_OnOpenSelector();

			void OnChangeSelection( void* ref, const Char* name );
		};
		struct AttrStruct
		{
			String					attrName;
			AttrType				attrType;
			String					selectorDataType;	// for single selector and list of selector
			DataArray<const Char*>  dropdownValues;		// for single dropdown and list of dropdown
			DataArray<AttrStruct>	structScheme;		// for list of struct
			AttrType				listType;			// for struct of list
		};


	public:
		AttrViewer();
		virtual ~AttrViewer() {}


	public:
		//! called once when the viewer is added to panel
		void Initialize		( AttributePanel* panel, QTreeWidget* panelTree );

		void ChangeTarget	( void* target );
		void UpdateGUI		();
		void Clear			();		
		void RefreshPanel	();
		void ShowViewer		();

		void A_OnChange_Group1		();
		void A_OnChange_Group2		();

		// factor methods that are linked to this AttrViewer
		QWidget* CreateString	( UInt flagMask, CallbackGroup callbackGroup );
		QWidget* CreateFloat	( UInt flagMask, CallbackGroup callbackGroup );
		QWidget* CreateInt		( UInt flagMask, CallbackGroup callbackGroup );
		QWidget* CreateBool		( UInt flagMask, CallbackGroup callbackGroup );
		QWidget* CreateVector2	( UInt flagMask, CallbackGroup callbackGroup );
		QWidget* CreateVector3	( UInt flagMask, CallbackGroup callbackGroup );
		QWidget* CreateVector4	( UInt flagMask, CallbackGroup callbackGroup );
		QWidget* CreateDropdown	( DataArray<const Char*>& valueList, 
								  UInt flagMask, CallbackGroup group );
		QWidget* CreateSelector	( const Char* dataType, UInt flagMask, CallbackGroup group );
		QWidget* CreateStruct	( DataArray<AttrStruct>& structScheme, 
								  UInt flagMask, CallbackGroup group );


	protected:
		virtual void _OnInitialize		()					PURE_VIRTUAL;	// it should create the attributes
		virtual void _OnChangeTarget	( void* target )	PURE_VIRTUAL;	// it should reset target
		virtual void _OnUpdateGUI		()					PURE_VIRTUAL;	// we should update the GUI
		virtual void _OnUpdateTarget	()					PURE_VIRTUAL;	// we should update the target


	protected:
		CallbackGroup	_activatedGroup;
		UInt			_ignoreUpdateGUI;

		void		_AddToPanel				( const Char* title );
		void		_UpdatePanel			();
		void		_UpdateVisibility		();

		// delete all attributes and groups
		void		_ResetViewer			();

		void		_OpenGroup				( const Char* name );
		void		_CloseGroup				();

		void		_CreateAttrLabel		( const Char* name, const Char* label );
		void		_CreateAttrString		( const Char* name, const Char* label );
		void		_CreateAttrBool			( const Char* name, const Char* label );
		void		_CreateAttrInt			( const Char* name, const Char* label );
		void		_CreateAttrFloat		( const Char* name, const Char* label );
		void		_CreateAttrVector2		( const Char* name, const Char* label );
		void		_CreateAttrVector3		( const Char* name, const Char* label );
		void		_CreateAttrVector4		( const Char* name, const Char* label );
		void		_CreateAttrDropdown		( const Char* name, const Char* label, 
											  DataArray<const Char*>& valueList );
		void		_CreateAttrSelector		( const Char* name, const Char* label, 
											  const Char* dataType );
		void		_CreateAttrStruct		( const Char* name, const Char* label, 
											  DataArray<AttrStruct>& structScheme );
		void		_CreateAttrList			( const Char* name, const Char* label, 
											  AttrType elementType );
		void		_CreateAttrListSelector	( const Char* name, const Char* label, 
											  const Char* dataType );
		void		_CreateAttrListDropdown	( const Char* name, const Char* label, 
											  DataArray<const Char*>& valueList );
		void		_CreateAttrListStruct	( const Char* name, const Char* label, 
											  DataArray<AttrStruct>& structScheme );

		void		_CreateAttrString		( const Char* name, const Char* label,
											  UInt flagMask, CallbackGroup group );
		void		_CreateAttrBool			( const Char* name, const Char* label,
											  UInt flagMask, CallbackGroup group );
		void		_CreateAttrInt			( const Char* name, const Char* label,
											  UInt flagMask, CallbackGroup group );
		void		_CreateAttrFloat		( const Char* name, const Char* label,
											  UInt flagMask, CallbackGroup group );
		void		_CreateAttrVector2		( const Char* name, const Char* label,
											  UInt flagMask, CallbackGroup group );
		void		_CreateAttrVector3		( const Char* name, const Char* label,
											  UInt flagMask, CallbackGroup group );
		void		_CreateAttrVector4		( const Char* name, const Char* label,
											  UInt flagMask, CallbackGroup group );
		void		_CreateAttrDropdown		( const Char* name, const Char* label,
											  DataArray<const Char*>& valueList,
											  UInt flagMask, CallbackGroup group );
		void		_CreateAttrSelector		( const Char* name, const Char* label,
											  const Char* dataType,
											  UInt flagMask, CallbackGroup group );
		void		_CreateAttrStruct		( const Char* name, const Char* label,
											  DataArray<AttrStruct>& structScheme,
											  UInt flagMask, CallbackGroup group );
		void		_CreateAttrList			( const Char* name, const Char* label,
											  AttrType elementType,
											  UInt flagMask, CallbackGroup group );
		void		_CreateAttrListSelector	( const Char* name, const Char* label,
											  const Char* dataType,
											  UInt flagMask, CallbackGroup group );
		void		_CreateAttrListDropdown	( const Char* name, const Char* label,
											  DataArray<const Char*>& valueList,
											  UInt flagMask, CallbackGroup group );
		void		_CreateAttrListStruct	( const Char* name, const Char* label,
											  DataArray<AttrStruct>& structScheme,
											  UInt flagMask, CallbackGroup group );
		
		void		_WriteAttrString		( const Char* name, const Char* value );
		void		_WriteAttrFloat			( const Char* name, Float value );
		void		_WriteAttrInt			( const Char* name, Int value );
		void		_WriteAttrBool			( const Char* name, Bool value );
		void		_WriteAttrVector2		( const Char* name, const Vector2& value );
		void		_WriteAttrVector3		( const Char* name, const Vector3& value );
		void		_WriteAttrVector4		( const Char* name, const Vector4& value );
		void		_WriteAttrDropdown		( const Char* name, UInt valueIndex );
		void		_WriteAttrSelector		( const Char* name, void* valueRef, 
											  const Char* valueName );
		void		_WriteAttrStruct		( const Char* name, const Char* elemName, 
											  const DataUnion& elemValue, const Char* valueName = NULL );
		void		_WriteAttrListSize		( const Char* name, UInt size );
		void		_WriteAttrListIndex		( const Char* name, Int index, DataUnion& elemValue,
											  const Char* valueName = NULL );
		void		_WriteAttrListIndex		( const Char* name, Int index, const Char* elemName, 
											  DataUnion& elemValue, const Char* valueName = NULL );
		void		_WriteAttrStrListSize	( const Char* name, const Char* listName, 
											  UInt size );
		void		_WriteAttrStrListIndex	( const Char* name, const Char* listName,
											  Int index, DataUnion& elemValue,
											  const Char* valueName = NULL );
		void		_WriteAttrStrListIndex	( const Char* name, const Char* listName, 
											  Int index, const Char* elemName, 
											  DataUnion& elemValue, const Char* valueName = NULL );

		String		_ReadAttrString			( const Char* name );
		Float		_ReadAttrFloat			( const Char* name );
		Int			_ReadAttrInt			( const Char* name );
		Bool		_ReadAttrBool			( const Char* name );
		Vector2		_ReadAttrVector2		( const Char* name );
		Vector3		_ReadAttrVector3		( const Char* name );
		Vector4		_ReadAttrVector4		( const Char* name );
		UInt		_ReadAttrDropdown		( const Char* name );
		void*		_ReadAttrSelector		( const Char* name );
		DataUnion	_ReadAttrStruct			( const Char* name, const Char* elemName );
		UInt		_ReadAttrListSize		( const Char* name );
		DataUnion	_ReadAttrListIndex		( const Char* name, Int index );
		DataUnion	_ReadAttrListIndex		( const Char* name, Int index, 
											  const Char* elemName );
		UInt		_ReadAttrStrListSize	( const Char* name, const Char* listName );
		DataUnion	_ReadAttrStrListIndex	( const Char* name, const Char* listName, 
											  Int index );
		DataUnion	_ReadAttrStrListIndex	( const Char* name, const Char* listName, 
											  Int index, const Char* elemName );

		void		_SetAttrVisibility		( const Char* name, Bool value );

		void		_CreateInnerAttribute	( InnerAttrType innerType );
		DataUnion	_ReadInnerAttribute		( InnerAttrType innerType );
		void		_WriteInnerAttribute	( InnerAttrType innerType, DataUnion& attrValue );

		void		_Colorize				( Bool enabled, Bool colorizeAll = FALSE );

		
	private:
		struct _Attribute
		{
			String				name;
			String				label;
			QTreeWidgetItem*	treeItem;
			CustomTreeWidget*	treeWidget;
			QWidget*			valueWidget;
			AttrType			type;
			Bool				isVisible;
		};
		struct _InnerAttribute
		{
			QWidget*	labelWidget;
			QWidget*	valueWidget;
			AttrType	type;
		};
		struct _Group
		{
			String	name;
			UInt	firstIndex;
			UInt	count;
		};
		struct _ListWidget;
		struct _StructWidget;

		DataArray<_Group>						_groups;
		DataArray<_Attribute>					_attributes;
		DataMap<InnerAttrType, _InnerAttribute>	_innerAttributes;

		AttributePanel*		_panel;
		QTreeWidget*		_panelTree;
		QTreeWidgetItem*	_titleItem;

		QWidget*			_titleWidget;
		QWidget*			_childWidget;

		void		_SetAttribute	( const Char* name, const Char* label, 
									  QWidget* widget, AttrType type );
		Bool		_GetAttribute	( const Char* name, OUT _Attribute& attribute );
		
		void		_WriteString		( QWidget* widget, const Char* value );
		void		_WriteFloat			( QWidget* widget, Float value );
		void		_WriteInt			( QWidget* widget, Int value );
		void		_WriteBool			( QWidget* widget, Bool value );
		void		_WriteVector2		( QWidget* widget, const Vector2& value );
		void		_WriteVector3		( QWidget* widget, const Vector3& value );
		void		_WriteVector4		( QWidget* widget, const Vector4& value );
		void		_WriteDropdown		( QWidget* widget, UInt valueIndex );
		void		_WriteSelector		( QWidget* widget, void* valueRef, 
										  const Char* valueName );
		void		_WriteListIndex		( QWidget* widget, Int index, const Char* elemName, 
										  DataUnion& elemValue, const Char* valueName = NULL );

		String		_ReadString			( QWidget* widget );
		Float		_ReadFloat			( QWidget* widget );
		Int			_ReadInt			( QWidget* widget );
		Bool		_ReadBool			( QWidget* widget );
		Vector2		_ReadVector2		( QWidget* widget );
		Vector3		_ReadVector3		( QWidget* widget );
		Vector4		_ReadVector4		( QWidget* widget );
		UInt		_ReadDropdown		( QWidget* widget );
		void*		_ReadSelector		( QWidget* widget );
		DataUnion	_ReadListIndex		( QWidget* widget, Int index, const Char* elemName );

		QWidget*	_CreateList			( AttrType elementType, UInt flagMask, 
										  CallbackGroup group );
		QWidget*	_CreateListDropdown	( DataArray<const Char*>& valueList, UInt flagMask, 
										  CallbackGroup group );
		QWidget*	_CreateListSelector	( const Char* dataType, UInt flagMask, 
										  CallbackGroup group );
		QWidget*	_CreateListStruct	( DataArray<AttrStruct>& structScheme, UInt flagMask, 
										  CallbackGroup group );

		QWidget*	_FindStructElem		( _StructWidget* structWidget, const Char* elemName,
										  OUT UInt& elemIndex );
	};
}