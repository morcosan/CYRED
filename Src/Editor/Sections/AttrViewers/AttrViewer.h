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

			void OnChangeSelection( void* ref, cchar* name );
		};
		struct AttrStruct
		{
			String					attrName;
			AttrType				attrType;
			String					selectorDataType;	// for single selector and list of selector
			DataArray<cchar*>  dropdownValues;		// for single dropdown and list of dropdown
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
		QWidget* CreateDropdown	( DataArray<cchar*>& valueList, 
								  int flagMask, CallbackGroup group );
		QWidget* CreateSelector	( cchar* dataType, int flagMask, CallbackGroup group );
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

		void		_AddToPanel				( cchar* title );
		void		_UpdatePanel			();
		void		_UpdateVisibility		();

		// delete all attributes and groups
		void		_ResetViewer			();

		void		_OpenGroup				( cchar* name );
		void		_CloseGroup				();

		void		_CreateAttrLabel		( cchar* name, cchar* label );
		void		_CreateAttrString		( cchar* name, cchar* label );
		void		_CreateAttrBool			( cchar* name, cchar* label );
		void		_CreateAttrInt			( cchar* name, cchar* label );
		void		_CreateAttrFloat		( cchar* name, cchar* label );
		void		_CreateAttrVector2		( cchar* name, cchar* label );
		void		_CreateAttrVector3		( cchar* name, cchar* label );
		void		_CreateAttrVector4		( cchar* name, cchar* label );
		void		_CreateAttrDropdown		( cchar* name, cchar* label, 
											  DataArray<cchar*>& valueList );
		void		_CreateAttrSelector		( cchar* name, cchar* label, 
											  cchar* dataType );
		void		_CreateAttrStruct		( cchar* name, cchar* label, 
											  DataArray<AttrStruct>& structScheme );
		void		_CreateAttrList			( cchar* name, cchar* label, 
											  AttrType elementType );
		void		_CreateAttrListSelector	( cchar* name, cchar* label, 
											  cchar* dataType );
		void		_CreateAttrListDropdown	( cchar* name, cchar* label, 
											  DataArray<cchar*>& valueList );
		void		_CreateAttrListStruct	( cchar* name, cchar* label, 
											  DataArray<AttrStruct>& structScheme );

		void		_CreateAttrString		( cchar* name, cchar* label,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrBool			( cchar* name, cchar* label,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrInt			( cchar* name, cchar* label,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrFloat		( cchar* name, cchar* label,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrVector2		( cchar* name, cchar* label,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrVector3		( cchar* name, cchar* label,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrVector4		( cchar* name, cchar* label,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrDropdown		( cchar* name, cchar* label,
											  DataArray<cchar*>& valueList,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrSelector		( cchar* name, cchar* label,
											  cchar* dataType,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrStruct		( cchar* name, cchar* label,
											  DataArray<AttrStruct>& structScheme,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrList			( cchar* name, cchar* label,
											  AttrType elementType,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrListSelector	( cchar* name, cchar* label,
											  cchar* dataType,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrListDropdown	( cchar* name, cchar* label,
											  DataArray<cchar*>& valueList,
											  int flagMask, CallbackGroup group );
		void		_CreateAttrListStruct	( cchar* name, cchar* label,
											  DataArray<AttrStruct>& structScheme,
											  int flagMask, CallbackGroup group );
		
		void		_WriteAttrString		( cchar* name, cchar* value );
		void		_WriteAttrFloat			( cchar* name, float value );
		void		_WriteAttrInt			( cchar* name, int value );
		void		_WriteAttrBool			( cchar* name, bool value );
		void		_WriteAttrVector2		( cchar* name, const Vector2& value );
		void		_WriteAttrVector3		( cchar* name, const Vector3& value );
		void		_WriteAttrVector4		( cchar* name, const Vector4& value );
		void		_WriteAttrDropdown		( cchar* name, int valueIndex );
		void		_WriteAttrSelector		( cchar* name, void* valueRef, 
											  cchar* valueName );
		void		_WriteAttrStruct		( cchar* name, cchar* elemName, 
											  const DataUnion& elemValue, cchar* valueName = NULL );
		void		_WriteAttrListSize		( cchar* name, int size );
		void		_WriteAttrListIndex		( cchar* name, int index, DataUnion& elemValue,
											  cchar* valueName = NULL );
		void		_WriteAttrListIndex		( cchar* name, int index, cchar* elemName, 
											  DataUnion& elemValue, cchar* valueName = NULL );
		void		_WriteAttrStrListSize	( cchar* name, cchar* listName, 
											  int size );
		void		_WriteAttrStrListIndex	( cchar* name, cchar* listName,
											  int index, DataUnion& elemValue,
											  cchar* valueName = NULL );
		void		_WriteAttrStrListIndex	( cchar* name, cchar* listName, 
											  int index, cchar* elemName, 
											  DataUnion& elemValue, cchar* valueName = NULL );

		String		_ReadAttrString			( cchar* name );
		float		_ReadAttrFloat			( cchar* name );
		int			_ReadAttrInt			( cchar* name );
		bool		_ReadAttrBool			( cchar* name );
		Vector2		_ReadAttrVector2		( cchar* name );
		Vector3		_ReadAttrVector3		( cchar* name );
		Vector4		_ReadAttrVector4		( cchar* name );
		int			_ReadAttrDropdown		( cchar* name );
		void*		_ReadAttrSelector		( cchar* name );
		DataUnion	_ReadAttrStruct			( cchar* name, cchar* elemName );
		int			_ReadAttrListSize		( cchar* name );
		DataUnion	_ReadAttrListIndex		( cchar* name, int index );
		DataUnion	_ReadAttrListIndex		( cchar* name, int index, 
											  cchar* elemName );
		int			_ReadAttrStrListSize	( cchar* name, cchar* listName );
		DataUnion	_ReadAttrStrListIndex	( cchar* name, cchar* listName, 
											  int index );
		DataUnion	_ReadAttrStrListIndex	( cchar* name, cchar* listName, 
											  int index, cchar* elemName );

		void		_SetAttrVisibility		( cchar* name, bool value );

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

		void		_SetAttribute	( cchar* name, cchar* label, 
									  QWidget* widget, AttrType type );
		bool		_GetAttribute	( cchar* name, OUT _Attribute& attribute );
		
		void		_WriteString		( QWidget* widget, cchar* value );
		void		_WriteFloat			( QWidget* widget, float value );
		void		_WriteInt			( QWidget* widget, int value );
		void		_WriteBool			( QWidget* widget, bool value );
		void		_WriteVector2		( QWidget* widget, const Vector2& value );
		void		_WriteVector3		( QWidget* widget, const Vector3& value );
		void		_WriteVector4		( QWidget* widget, const Vector4& value );
		void		_WriteDropdown		( QWidget* widget, int valueIndex );
		void		_WriteSelector		( QWidget* widget, void* valueRef, 
										  cchar* valueName );
		void		_WriteListIndex		( QWidget* widget, int index, cchar* elemName, 
										  DataUnion& elemValue, cchar* valueName = NULL );

		String		_ReadString			( QWidget* widget );
		float		_ReadFloat			( QWidget* widget );
		int			_ReadInt			( QWidget* widget );
		bool		_ReadBool			( QWidget* widget );
		Vector2		_ReadVector2		( QWidget* widget );
		Vector3		_ReadVector3		( QWidget* widget );
		Vector4		_ReadVector4		( QWidget* widget );
		int			_ReadDropdown		( QWidget* widget );
		void*		_ReadSelector		( QWidget* widget );
		DataUnion	_ReadListIndex		( QWidget* widget, int index, cchar* elemName );

		QWidget*	_CreateList			( AttrType elementType, int flagMask, 
										  CallbackGroup group );
		QWidget*	_CreateListDropdown	( DataArray<cchar*>& valueList, int flagMask, 
										  CallbackGroup group );
		QWidget*	_CreateListSelector	( cchar* dataType, int flagMask, 
										  CallbackGroup group );
		QWidget*	_CreateListStruct	( DataArray<AttrStruct>& structScheme, int flagMask, 
										  CallbackGroup group );

		QWidget*	_FindStructElem		( _StructWidget* structWidget, cchar* elemName,
										  OUT int& elemIndex );
	};
}