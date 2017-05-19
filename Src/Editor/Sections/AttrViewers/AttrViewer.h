// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
	class Panel_Attributes;
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

			void OnChangeSelection( void* ref, const char* name );
		};
		struct AttrStruct
		{
			String					attrName;
			AttrType				attrType;
			String					selectorDataType;	// for single selector and list of selector
			DataArray<const char*>  dropdownValues;		// for single dropdown and list of dropdown
			DataArray<AttrStruct>	structScheme;		// for list of struct
			AttrType				listType;			// for struct of list
		};


	public:
		AttrViewer();
		virtual ~AttrViewer() {}


	public:
		//! called once when the viewer is added to panel
		void Initialize		( Panel_Attributes* panel, QTreeWidget* panelTree );

		void ChangeTarget	( void* target );
		void UpdateGUI		();
		void Clear			();		
		void RefreshPanel	();
		void ShowViewer		();

		void A_OnChange_Group1		();
		void A_OnChange_Group2		();

		// factor methods that are linked to this AttrViewer
		QWidget* CreateString	( int flagMask, CallbackGroup callbackGroup );
		QWidget* CreateFloat	( int flagMask, CallbackGroup callbackGroup );
		QWidget* CreateInt		( int flagMask, CallbackGroup callbackGroup );
		QWidget* CreateBool		( int flagMask, CallbackGroup callbackGroup );
		QWidget* CreateVector2	( int flagMask, CallbackGroup callbackGroup );
		QWidget* CreateVector3	( int flagMask, CallbackGroup callbackGroup );
		QWidget* CreateVector4	( int flagMask, CallbackGroup callbackGroup );
		QWidget* CreateDropdown	( DataArray<const char*>& valueList, 
								  int flagMask, CallbackGroup group );
		QWidget* CreateSelector	( const char* dataType, int flagMask, CallbackGroup group );
		QWidget* CreateStruct	( DataArray<AttrStruct>& structScheme, 
								  int flagMask, CallbackGroup group );


	protected:
		virtual void _OnInitialize		()					PURE_VIRTUAL;	// it should create the attributes
		virtual void _OnChangeTarget	( void* target )	PURE_VIRTUAL;	// it should reset target
		virtual void _OnUpdateGUI		()					PURE_VIRTUAL;	// we should update the GUI
		virtual void _OnUpdateTarget	()					PURE_VIRTUAL;	// we should update the target


	protected:
		CallbackGroup	_activatedGroup;
		int				_ignoreUpdateGUI;

		void		_AddToPanel				( const char* title );
		void		_UpdatePanel			();
		void		_UpdateVisibility		();

		// delete all attributes and groups
		void		_ResetViewer			();

		void		_OpenGroup				( const char* name );
		void		_CloseGroup				();

		void		_CreateAttrLabel		( const char* name, const char* label );
		void		_CreateAttrString		( const char* name, const char* label );
		void		_CreateAttrBool			( const char* name, const char* label );
		void		_CreateAttrInt			( const char* name, const char* label );
		void		_CreateAttrFloat		( const char* name, const char* label );
		void		_CreateAttrVector2		( const char* name, const char* label );
		void		_CreateAttrVector3		( const char* name, const char* label );
		void		_CreateAttrVector4		( const char* name, const char* label );
		void		_CreateAttrDropdown		( const char* name, const char* label, 
											  DataArray<const char*>& valueList );
		void		_CreateAttrSelector		( const char* name, const char* label, 
											  const char* dataType );
		void		_CreateAttrStruct		( const char* name, const char* label, 
											  DataArray<AttrStruct>& structScheme );
		void		_CreateAttrList			( const char* name, const char* label, 
											  AttrType elementType );
		void		_CreateAttrListSelector	( const char* name, const char* label, 
											  const char* dataType );
		void		_CreateAttrListDropdown	( const char* name, const char* label, 
											  DataArray<const char*>& valueList );
		void		_CreateAttrListStruct	( const char* name, const char* label, 
											  DataArray<AttrStruct>& structScheme );

		void		_CreateAttrString		( const char* name, const char* label,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrBool			( const char* name, const char* label,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrInt			( const char* name, const char* label,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrFloat		( const char* name, const char* label,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrVector2		( const char* name, const char* label,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrVector3		( const char* name, const char* label,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrVector4		( const char* name, const char* label,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrDropdown		( const char* name, const char* label,
											  DataArray<const char*>& valueList,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrSelector		( const char* name, const char* label,
											  const char* dataType,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrStruct		( const char* name, const char* label,
											  DataArray<AttrStruct>& structScheme,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrList			( const char* name, const char* label,
											  AttrType elementType,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrListSelector	( const char* name, const char* label,
											  const char* dataType,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrListDropdown	( const char* name, const char* label,
											  DataArray<const char*>& valueList,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrListStruct	( const char* name, const char* label,
											  DataArray<AttrStruct>& structScheme,
											  int flagMask, CallbackGroup group );
		
		void		_WriteAttrString		( const char* name, const char* value );
		void		_WriteAttrFloat			( const char* name, float value );
		void		_WriteAttrInt			( const char* name, int value );
		void		_WriteAttrBool			( const char* name, bool value );
		void		_WriteAttrVector2		( const char* name, const Vector2& value );
		void		_WriteAttrVector3		( const char* name, const Vector3& value );
		void		_WriteAttrVector4		( const char* name, const Vector4& value );
		void		_WriteAttrDropdown		( const char* name, int valueIndex );
		void		_WriteAttrSelector		( const char* name, void* valueRef, 
											  const char* valueName );
		void		_WriteAttrStruct		( const char* name, const char* elemName, 
											  const DataUnion& elemValue, const char* valueName = NULL );
		void		_WriteAttrListSize		( const char* name, int size );
		void		_WriteAttrListIndex		( const char* name, int index, DataUnion& elemValue,
											  const char* valueName = NULL );
		void		_WriteAttrListIndex		( const char* name, int index, const char* elemName, 
											  DataUnion& elemValue, const char* valueName = NULL );
		void		_WriteAttrStrListSize	( const char* name, const char* listName, 
											  int size );
		void		_WriteAttrStrListIndex	( const char* name, const char* listName,
											  int index, DataUnion& elemValue,
											  const char* valueName = NULL );
		void		_WriteAttrStrListIndex	( const char* name, const char* listName, 
											  int index, const char* elemName, 
											  DataUnion& elemValue, const char* valueName = NULL );

		String		_ReadAttrString			( const char* name );
		float		_ReadAttrFloat			( const char* name );
		int			_ReadAttrInt			( const char* name );
		bool		_ReadAttrBool			( const char* name );
		Vector2		_ReadAttrVector2		( const char* name );
		Vector3		_ReadAttrVector3		( const char* name );
		Vector4		_ReadAttrVector4		( const char* name );
		int			_ReadAttrDropdown		( const char* name );
		void*		_ReadAttrSelector		( const char* name );
		DataUnion	_ReadAttrStruct			( const char* name, const char* elemName );
		int			_ReadAttrListSize		( const char* name );
		DataUnion	_ReadAttrListIndex		( const char* name, int index );
		DataUnion	_ReadAttrListIndex		( const char* name, int index, 
											  const char* elemName );
		int			_ReadAttrStrListSize	( const char* name, const char* listName );
		DataUnion	_ReadAttrStrListIndex	( const char* name, const char* listName, 
											  int index );
		DataUnion	_ReadAttrStrListIndex	( const char* name, const char* listName, 
											  int index, const char* elemName );

		void		_SetAttrVisibility		( const char* name, bool value );

		void		_CreateInnerAttribute	( InnerAttrType innerType );
		DataUnion	_ReadInnerAttribute		( InnerAttrType innerType );
		void		_WriteInnerAttribute	( InnerAttrType innerType, DataUnion& attrValue );

		void		_Colorize				( bool enabled, bool colorizeAll = FALSE );

		
	private:
		struct _Attribute
		{
			String				name;
			String				label;
			QTreeWidgetItem*	treeItem;
			CustomTreeWidget*	treeWidget;
			QWidget*			valueWidget;
			AttrType			type;
			bool				isVisible;
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
			int	firstIndex;
			int	count;
		};
		struct _ListWidget;
		struct _StructWidget;

		DataArray<_Group>						_groups;
		DataArray<_Attribute>					_attributes;
		DataMap<InnerAttrType, _InnerAttribute>	_innerAttributes;

		Panel_Attributes*		_panel;
		QTreeWidget*		_panelTree;
		QTreeWidgetItem*	_titleItem;

		QWidget*			_titleWidget;
		QWidget*			_childWidget;

		void		_SetAttribute	( const char* name, const char* label, 
									  QWidget* widget, AttrType type );
		bool		_GetAttribute	( const char* name, OUT _Attribute& attribute );
		
		void		_WriteString		( QWidget* widget, const char* value );
		void		_WriteFloat			( QWidget* widget, float value );
		void		_WriteInt			( QWidget* widget, int value );
		void		_WriteBool			( QWidget* widget, bool value );
		void		_WriteVector2		( QWidget* widget, const Vector2& value );
		void		_WriteVector3		( QWidget* widget, const Vector3& value );
		void		_WriteVector4		( QWidget* widget, const Vector4& value );
		void		_WriteDropdown		( QWidget* widget, int valueIndex );
		void		_WriteSelector		( QWidget* widget, void* valueRef, 
										  const char* valueName );
		void		_WriteListIndex		( QWidget* widget, int index, const char* elemName, 
										  DataUnion& elemValue, const char* valueName = NULL );

		String		_ReadString			( QWidget* widget );
		float		_ReadFloat			( QWidget* widget );
		int			_ReadInt			( QWidget* widget );
		bool		_ReadBool			( QWidget* widget );
		Vector2		_ReadVector2		( QWidget* widget );
		Vector3		_ReadVector3		( QWidget* widget );
		Vector4		_ReadVector4		( QWidget* widget );
		int			_ReadDropdown		( QWidget* widget );
		void*		_ReadSelector		( QWidget* widget );
		DataUnion	_ReadListIndex		( QWidget* widget, int index, const char* elemName );

		QWidget*	_CreateList			( AttrType elementType, int flagMask, 
										  CallbackGroup group );
		QWidget*	_CreateListDropdown	( DataArray<const char*>& valueList, int flagMask, 
										  CallbackGroup group );
		QWidget*	_CreateListSelector	( const char* dataType, int flagMask, 
										  CallbackGroup group );
		QWidget*	_CreateListStruct	( DataArray<AttrStruct>& structScheme, int flagMask, 
										  CallbackGroup group );

		QWidget*	_FindStructElem		( _StructWidget* structWidget, const char* elemName,
										  OUT int& elemIndex );
	};
}