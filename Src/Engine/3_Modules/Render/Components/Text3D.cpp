// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Text3D.h"

#include "../Assets/Font.h"
#include "../../Event/EventManager.h"


using namespace CYRED;


const int Text3D::RENDER_UNIT_SIZE = 12;


Text3D::Text3D( GameObject* gameObject )
	: Component( gameObject )
	, _textColor( Vector4( 1, 1, 1, 1 ) )
	, _font( NULL )
	, _textSize( RENDER_UNIT_SIZE )
	, _verticalAlign( VerticalAlign::TOP )
	, _horizontalAlign( HorizontalAlign::LEFT )
	, _material( NULL )
{
	_componentType = ComponentType::TEXT_3D;
}


cchar* Text3D::GetText() const
{
	return _text.GetChar();
}


int Text3D::GetTextSize() const
{
	return _textSize;
}


Vector4 Text3D::GetTextColor() const
{
	return _textColor;
}


Font* Text3D::GetFont() const
{
	return _font;
}


VerticalAlign Text3D::GetVerticalAlign() const
{
	return _verticalAlign;
}


HorizontalAlign Text3D::GetHorizontalAlign() const
{
	return _horizontalAlign;
}


Material* Text3D::GetMaterial() const
{
	return _material;
}


void Text3D::SetText( cchar* text )
{
	_text = text;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::COMPONENT_UPDATE, this );
	}
}


void Text3D::SetTextSize( int size )
{
	_textSize = size;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::COMPONENT_UPDATE, this );
	}
}


void Text3D::SetTextColor( const Vector4& color )
{
	_textColor = color;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::COMPONENT_UPDATE, this );
	}
}


void Text3D::SetFont( Font* font )
{
	_font = font;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::COMPONENT_UPDATE, this );
	}
}


void Text3D::SetVerticalAlign( VerticalAlign align )
{
	_verticalAlign = align;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::COMPONENT_UPDATE, this );
	}
}


void Text3D::SetHorizontalAlign( HorizontalAlign align )
{
	_horizontalAlign = align;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::COMPONENT_UPDATE, this );
	}
}


void Text3D::SetMaterial( Material* material )
{
	_material = material;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::COMPONENT_UPDATE, this );
	}
}
