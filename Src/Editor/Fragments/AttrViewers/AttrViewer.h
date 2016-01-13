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
}


namespace CYRED
{
	namespace Enum_AttrType
	{
		enum Enum
		{
			STRING
			, FLOAT
			, INT
			, BOOL
			, VECTOR2
			, VECTOR3
			, VECTOR4
			, DROPDOWN
			, SELECTOR
			, LIST
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


	public:
		AttrViewer();
		virtual ~AttrViewer() {}


	public:
		virtual void OnSelect_Target( void* target )	PURE_VIRTUAL;	// we should create the attributes
		
		void UpdateGUI		();
		void Clear			();		
		void RefreshPanel	();
		//! called once when the viewer is added to panel
		void UsePanel		( QTreeWidget* panelTree );		// add all elements to this panel

		void A_OnChange_Group1		();
		void A_OnChange_Group2		();


	protected:
		virtual void _OnUpdate_GUI		()	PURE_VIRTUAL;	// we should update the GUI
		virtual void _OnUpdate_Target	()	PURE_VIRTUAL;	// we should update the target


	protected:
		CallbackGroup	_activatedGroup;

		void		_AddToPanel				( const Char* title );
		void		_UpdateVisibility		();

		void		_OpenGroup				( const Char* name );
		void		_CloseGroup				();

		void		_CreateAttrString		( const Char* name );
		void		_CreateAttrBool			( const Char* name );
		void		_CreateAttrInt			( const Char* name );
		void		_CreateAttrFloat		( const Char* name );
		void		_CreateAttrVector2		( const Char* name );
		void		_CreateAttrVector3		( const Char* name );
		void		_CreateAttrVector4		( const Char* name );
		void		_CreateAttrDropdown		( const Char* name, DataArray<const Char*>& valueList );
		void		_CreateAttrSelector		( const Char* name, const Char* dataType );
		void		_CreateAttrList			( const Char* name, AttrType elementType );

		void		_CreateAttrString		( const Char* name, UInt flagMask, CallbackGroup group );
		void		_CreateAttrBool			( const Char* name, UInt flagMask, CallbackGroup group );
		void		_CreateAttrInt			( const Char* name, UInt flagMask, CallbackGroup group );
		void		_CreateAttrFloat		( const Char* name, UInt flagMask, CallbackGroup group );
		void		_CreateAttrVector2		( const Char* name, UInt flagMask, CallbackGroup group );
		void		_CreateAttrVector3		( const Char* name, UInt flagMask, CallbackGroup group );
		void		_CreateAttrVector4		( const Char* name, UInt flagMask, CallbackGroup group );
		void		_CreateAttrDropdown		( const Char* name, DataArray<const Char*>& valueList,
											  UInt flagMask, CallbackGroup group );
		void		_CreateAttrSelector		( const Char* name, const Char* dataType,
											  UInt flagMask, CallbackGroup group );
		
		void		_SetAttrVisibility		( const Char* name, Bool value );

		DataUnion	_ReadAttribute			( const Char* name );
		void		_WriteAttribute			( const Char* name, DataUnion& attrValue );
		void		_WriteAttribute			( const Char* name, DataUnion& attrValue, 
											  const Char* attrValueName );
		
		void		_CreateInnerAttribute	( InnerAttrType innerType );
		DataUnion	_ReadInnerAttribute		( InnerAttrType innerType );
		void		_WriteInnerAttribute	( InnerAttrType innerType, DataUnion& attrValue );

		void		_Colorize				( Bool enabled, Bool colorizeAll = FALSE );

		
	private:
		struct _Attribute
		{
			String				name;
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
		struct _ListSizeField;

		DataArray<_Group>						_groups;
		DataArray<_Attribute>					_attributes;
		DataMap<InnerAttrType, _InnerAttribute>	_innerAttributes;

		QTreeWidget*	_panelTree;
		QWidget*		_titleWidget;
		QWidget*		_childWidget;

		QTreeWidgetItem* _childItem;
		QTreeWidgetItem* _titleItem;

		void		_SetAttribute	( const Char* name, QWidget* widget, AttrType type );
		Bool		_GetAttribute	( const Char* name, OUT _Attribute& attribute );

		DataUnion	_ReadAttr		( _Attribute attr );
		void		_WriteAttr		( _Attribute attr, DataUnion& attrValue,
									  const Char* attrValueName );

		QWidget* _CreateString	( UInt flagMask, CallbackGroup callbackGroup );
		QWidget* _CreateFloat	( UInt flagMask, CallbackGroup callbackGroup );
		QWidget* _CreateInt		( UInt flagMask, CallbackGroup callbackGroup );
		QWidget* _CreateBool	( UInt flagMask, CallbackGroup callbackGroup );
		QWidget* _CreateVector2	( UInt flagMask, CallbackGroup callbackGroup );
		QWidget* _CreateVector3	( UInt flagMask, CallbackGroup callbackGroup );
		QWidget* _CreateVector4	( UInt flagMask, CallbackGroup callbackGroup );
		QWidget* _CreateDropdown( DataArray<const Char*>& valueList, UInt flagMask,
								  CallbackGroup group );
		QWidget* _CreateSelector( const Char* dataType, UInt flagMask, CallbackGroup group );
		QWidget* _CreateList	( AttrType elementType, UInt flagMask, CallbackGroup group );

		void _WriteString	( QWidget* widget, const Char* value );
		void _WriteFloat	( QWidget* widget, Float value );
		void _WriteInt		( QWidget* widget, Int value );
		void _WriteBool		( QWidget* widget, Bool value );
		void _WriteVector2	( QWidget* widget, const Vector2& value );
		void _WriteVector3	( QWidget* widget, const Vector3& value );
		void _WriteVector4	( QWidget* widget, const Vector4& value );
		void _WriteDropdown	( QWidget* widget, UInt valueIndex );
		void _WriteSelector	( QWidget* widget, void* valueRef, const Char* valueName );

		String	_ReadString		( QWidget* widget );
		Float	_ReadFloat		( QWidget* widget );
		Int		_ReadInt		( QWidget* widget );
		Bool	_ReadBool		( QWidget* widget );
		Vector2	_ReadVector2	( QWidget* widget );
		Vector3	_ReadVector3	( QWidget* widget );
		Vector4	_ReadVector4	( QWidget* widget );
		UInt	_ReadDropdown	( QWidget* widget );
		void*	_ReadSelector	( QWidget* widget );
	};
}