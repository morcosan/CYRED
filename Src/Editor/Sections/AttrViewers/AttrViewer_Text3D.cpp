// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Text3D.h"

#include "CyredModule_Render.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Event.h"

#include "../Selectors/Selector_Font.h"
#include "../Selectors/Selector_Material.h"


using namespace CYRED;


void AttrViewer_Text3D::_OnInitialize()
{
	_CreateAttrString	( ATTR_TEXT,		ATTR_TEXT );
	_CreateAttrInt		( ATTR_TEXT_SIZE,	ATTR_TEXT_SIZE );
	_CreateAttrVector4	( ATTR_TEXT_COLOR,	ATTR_TEXT_COLOR );

	DataArray<cchar*> hAlign;
	hAlign.Add( H_ALIN_LEFT );
	hAlign.Add( H_ALIN_MIDDLE );
	hAlign.Add( H_ALIN_RIGHT );
	_CreateAttrDropdown	( ATTR_H_ALIGN, ATTR_H_ALIGN, hAlign );

	DataArray<cchar*> vAlign;
	vAlign.Add( V_ALIN_TOP );
	vAlign.Add( V_ALIN_MIDDLE );
	vAlign.Add( V_ALIN_BOTTOM );
	_CreateAttrDropdown	( ATTR_V_ALIGN, ATTR_V_ALIGN, vAlign );

	_CreateAttrSelector	( ATTR_FONT,		ATTR_FONT,		Selector_Font::TYPE );
	_CreateAttrSelector	( ATTR_MATERIAL,	ATTR_MATERIAL,	Selector_Material::TYPE );

	_CreateInnerAttribute( InnerAttrType::ENABLED );
	_CreateInnerAttribute( InnerAttrType::SETTINGS );

	_AddToPanel( TITLE );
}


void AttrViewer_Text3D::_OnChangeTarget( void* target )
{
	_target = CAST_S( Text3D*, target );

	// prepare settings
	DataUnion attr;
	_WriteInnerAttribute( InnerAttrType::SETTINGS, attr.SetReference( _target ) );

	// change color for enable
	_Colorize( _target->IsEnabled() );
}


void AttrViewer_Text3D::_UpdateGUI()
{
	_WriteAttrString	( ATTR_TEXT,		_target->GetText() );
	_WriteAttrInt		( ATTR_TEXT_SIZE,	_target->GetTextSize() );
	_WriteAttrVector4	( ATTR_TEXT_COLOR,	_target->GetTextColor() );

	{
		int typeIndex = 0;
		switch ( _target->GetVerticalAlign() ) {
			case VerticalAlign::TOP:	typeIndex = 0;	break;
			case VerticalAlign::MIDDLE:	typeIndex = 1;	break;
			case VerticalAlign::BOTTOM:	typeIndex = 2;	break;
		}
		_WriteAttrDropdown( ATTR_V_ALIGN, typeIndex );
	}
	{
		int typeIndex = 0;
		switch ( _target->GetHorizontalAlign() ) {
			case HorizontalAlign::LEFT:		typeIndex = 0;	break;
			case HorizontalAlign::MIDDLE:	typeIndex = 1;	break;
			case HorizontalAlign::RIGHT:	typeIndex = 2;	break;
		}
		_WriteAttrDropdown( ATTR_H_ALIGN, typeIndex );
	}

	Font* font = _target->GetFont();
	cchar* fontName = (font == NULL) ? Selector_Font::OPTION_NULL : font->GetName();
	_WriteAttrSelector( ATTR_FONT, font, fontName );

	Material* material = _target->GetMaterial();
	cchar* matName = (material == NULL) ? Selector_Material::OPTION_NULL : material->GetName();
	_WriteAttrSelector( ATTR_MATERIAL, material, matName );

	if ( _target->IsEnabled() != _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() ) {
		DataUnion attr;
		_WriteInnerAttribute( InnerAttrType::ENABLED, attr.SetBool( _target->IsEnabled() ) );
		// change color for enable
		_Colorize( _target->IsEnabled() );
	}
}


void AttrViewer_Text3D::_UpdateTarget()
{
	_target->SetEmitEvents( FALSE );
	{
		_target->SetText		( _ReadAttrString( ATTR_TEXT ).GetChar() );

		int vAlign = _ReadAttrDropdown( ATTR_V_ALIGN );
		switch ( vAlign ) {
			case 0:	_target->SetVerticalAlign( VerticalAlign::TOP );	break;
			case 1:	_target->SetVerticalAlign( VerticalAlign::MIDDLE );	break;
			case 2:	_target->SetVerticalAlign( VerticalAlign::BOTTOM );	break;
		}

		int hAlign = _ReadAttrDropdown( ATTR_H_ALIGN );
		switch ( hAlign ) {
			case 0:	_target->SetHorizontalAlign( HorizontalAlign::LEFT );	break;
			case 1:	_target->SetHorizontalAlign( HorizontalAlign::MIDDLE );	break;
			case 2:	_target->SetHorizontalAlign( HorizontalAlign::RIGHT );	break;
		}

		_target->SetTextSize	( _ReadAttrInt( ATTR_TEXT_SIZE ) );
		_target->SetTextColor	( _ReadAttrVector4( ATTR_TEXT_COLOR ) );
		_target->SetFont		( CAST_S( Font*, _ReadAttrSelector( ATTR_FONT ) ) );
		_target->SetMaterial	( CAST_S( Material*, _ReadAttrSelector( ATTR_MATERIAL ) ) );
		
		bool newValue = _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool();
		if ( _target->IsEnabled() != newValue ) {
			_target->SetEnabled( newValue );
			// change color for enable
			_Colorize( _target->IsEnabled() );
		}
	}
	_target->SetEmitEvents( TRUE );

	// emit event manually
	++_ignoreUpdateGUI;
	EventManager::Singleton()->PushEvent( EventType::COMPONENT_UPDATE, _target );

	// update ui
	_Colorize( _target->IsEnabled() );
}
