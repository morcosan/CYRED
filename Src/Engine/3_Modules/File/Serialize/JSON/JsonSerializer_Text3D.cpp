// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Text3D.h"

#include "../../../Render/Components/Text3D.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"
#include "../../../Render/Assets/Material.h"
#include "../../../Render/Assets/Font.h"
#include "../../../Asset/AssetManager.h"
#include "../../../../2_BuildingBlocks/Random/Random.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;


rapidjson::Value JsonSerializer_Text3D::ToJson( const void* object )
{
	const Text3D* text3D = CAST_S( const Text3D*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( ATTR_ENABLED ),	
					text3D->IsEnabled(),	
					_al );

	json.AddMember( rapidjson::StringRef( ATTR_TEXT ),	
					rapidjson::StringRef( text3D->GetText() ),	
					_al );
	json.AddMember( rapidjson::StringRef( ATTR_TEXT_SIZE ), 
					text3D->GetTextSize(), 
					_al );
	json.AddMember( rapidjson::StringRef( ATTR_TEXT_COLOR ), 
					_ToJsonVec4( text3D->GetTextColor() ), 
					_al );

	switch ( text3D->GetVerticalAlign() ) {
		case VerticalAlign::TOP:
			json.AddMember( rapidjson::StringRef( ATTR_V_ALIGN ),
							rapidjson::StringRef( V_ALIN_TOP ),
							_al );
			break;

		case VerticalAlign::MIDDLE:
			json.AddMember( rapidjson::StringRef( ATTR_V_ALIGN ),
							rapidjson::StringRef( V_ALIN_MIDDLE ),
							_al );
			break;

		case VerticalAlign::BOTTOM:
			json.AddMember( rapidjson::StringRef( ATTR_V_ALIGN ),
							rapidjson::StringRef( V_ALIN_BOTTOM ),
							_al );
			break;
	}

	switch ( text3D->GetHorizontalAlign() ) {
		case VerticalAlign::TOP:
			json.AddMember( rapidjson::StringRef( ATTR_H_ALIGN ),
							rapidjson::StringRef( H_ALIN_LEFT ),
							_al );
			break;

		case VerticalAlign::MIDDLE:
			json.AddMember( rapidjson::StringRef( ATTR_H_ALIGN ),
							rapidjson::StringRef( H_ALIN_MIDDLE ),
							_al );
			break;

		case VerticalAlign::BOTTOM:
			json.AddMember( rapidjson::StringRef( ATTR_H_ALIGN ),
							rapidjson::StringRef( H_ALIN_RIGHT ),
							_al );
			break;
	}

	{
		Font* font = text3D->GetFont();
		if ( font == NULL )	{
			rapidjson::Value nullNode;
			nullNode.SetNull();
			json.AddMember( rapidjson::StringRef( ATTR_FONT ), nullNode, _al );
		}
		else {
			json.AddMember( rapidjson::StringRef( ATTR_FONT ),
							rapidjson::StringRef( font->GetUniqueID() ),
							_al );
		}
	}
	{
		Material* material = text3D->GetMaterial();
		if ( material == NULL )	{
			rapidjson::Value nullNode;
			nullNode.SetNull();
			json.AddMember( rapidjson::StringRef( ATTR_MATERIAL ), nullNode, _al );
		}
		else {
			json.AddMember( rapidjson::StringRef( ATTR_MATERIAL ),
							rapidjson::StringRef( material->GetUniqueID() ),
							_al );
		}
	}

	return json;
}


void JsonSerializer_Text3D::FromJson( rapidjson::Value& json, OUT void* object, 
										 DeserFlag flag )
{
	Text3D* text3D = CAST_S( Text3D*, object );

	bool emitEvents = text3D->DoesEmitEvents();
	text3D->SetEmitEvents( FALSE );


	if ( json.HasMember( ATTR_ENABLED ) ) {
		text3D->SetEnabled( json[ATTR_ENABLED].GetBool() );
	}

	if ( json.HasMember( ATTR_TEXT ) ) {
		text3D->SetText( json[ATTR_TEXT].GetString() );
	}

	if ( json.HasMember( ATTR_TEXT_SIZE ) ) {
		text3D->SetTextSize( json[ATTR_TEXT_SIZE].GetInt() );
	}

	if ( json.HasMember( ATTR_TEXT_COLOR ) ) {
		text3D->SetTextColor( _FromJsonVec4( json[ATTR_TEXT_COLOR] ) );
	}

	if ( json.HasMember( ATTR_V_ALIGN ) ) {
		FiniteString align( json[ATTR_V_ALIGN].GetString() );

		if ( align == V_ALIN_TOP ) {
			text3D->SetVerticalAlign( VerticalAlign::TOP );
		}
		else if ( align == V_ALIN_MIDDLE ) {
			text3D->SetVerticalAlign( VerticalAlign::MIDDLE );
		}
		else if ( align == V_ALIN_BOTTOM ) {
			text3D->SetVerticalAlign( VerticalAlign::BOTTOM );
		}
	}

	if ( json.HasMember( ATTR_H_ALIGN ) ) {
		FiniteString align( json[ATTR_H_ALIGN].GetString() );

		if ( align == H_ALIN_LEFT ) {
			text3D->SetHorizontalAlign( HorizontalAlign::LEFT );
		}
		else if ( align == H_ALIN_MIDDLE ) {
			text3D->SetHorizontalAlign( HorizontalAlign::MIDDLE );
		}
		else if ( align == H_ALIN_RIGHT ) {
			text3D->SetHorizontalAlign( HorizontalAlign::RIGHT );
		}
	}

	if ( json.HasMember( ATTR_FONT ) ) {
		if ( json[ATTR_FONT].IsNull() )	{
			text3D->SetFont( NULL );
		}
		else {
			cchar* uniqueID = json[ATTR_FONT].GetString();
			Font* font = AssetManager::Singleton()->GetFont( uniqueID );
			if ( font == NULL )	{
				bool isOk = Random::ValidateUniqueID( uniqueID );
				if ( isOk )	{
					font = new Font();
					font->SetUniqueID( uniqueID );
					AssetManager::Singleton()->AddFont( font );
				}
			}
			text3D->SetFont( font );
		}
	}

	if ( json.HasMember( ATTR_MATERIAL ) ) {
		if ( json[ATTR_MATERIAL].IsNull() )	{
			text3D->SetMaterial( NULL );
		}
		else {
			cchar* uniqueID = json[ATTR_MATERIAL].GetString();
			Material* material = AssetManager::Singleton()->GetMaterial( uniqueID );
			if ( material == NULL )	{
				bool isOk = Random::ValidateUniqueID( uniqueID );
				if ( isOk )	{
					material = new Material();
					material->SetUniqueID( uniqueID );
					AssetManager::Singleton()->AddMaterial( material );
				}
			}
			text3D->SetMaterial( material );
		}
	}


	text3D->SetEmitEvents( emitEvents );
}
