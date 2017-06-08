// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Texture.h"
#include "../../../Render/Assets/Texture.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_Texture::ToJson( const void* object )
{
	const Texture* texture = CAST_S( const Texture*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( ATTR_UNIQUE_ID ),
					rapidjson::StringRef( texture->GetUniqueID() ),
					_al );

	switch ( texture->GetTextureType() ) {
		case TextureType::TEXTURE_2D:
			json.AddMember( rapidjson::StringRef( ATTR_TEXTURE_TYPE ),
							rapidjson::StringRef( TYPE_TEXTURE_2D ),
							_al );
			break;

		case TextureType::CUBE_MAP:
			json.AddMember( rapidjson::StringRef( ATTR_TEXTURE_TYPE ),
							rapidjson::StringRef( TYPE_CUBE_MAP ),
							_al );
			break;
	}

	switch ( texture->GetLoadType() ) {
		case TextureLoadType::SCRIPTED:
			json.AddMember( rapidjson::StringRef( ATTR_LOAD_TYPE ),
							rapidjson::StringRef( TYPE_SCRIPTED ),
							_al );
			break;

		case TextureLoadType::EXTERNAL:
			json.AddMember( rapidjson::StringRef( ATTR_LOAD_TYPE ),
							rapidjson::StringRef( TYPE_EXTERNAL ),
							_al );
			break;
	}

	json.AddMember( rapidjson::StringRef( ATTR_HAS_MIPMAP ),
					texture->HasMipmap(),
					_al );
	json.AddMember( rapidjson::StringRef( ATTR_CLEAR_BUFFER ),
					texture->DoesClearBufferOnBind(),
					_al );

	switch ( texture->GetTextureType() )
	{
		case TextureType::TEXTURE_2D:
			json.AddMember( rapidjson::StringRef( ATTR_FILE_PATH ),
							rapidjson::StringRef( texture->GetImagePath(0) ),
							_al );
			break;

		case TextureType::CUBE_MAP:
			json.AddMember( rapidjson::StringRef( ATTR_FILE_PATH_POSX ),
							rapidjson::StringRef( texture->GetImagePath(0) ),
							_al );
			json.AddMember( rapidjson::StringRef( ATTR_FILE_PATH_NEGX ),
							rapidjson::StringRef( texture->GetImagePath(1) ),
							_al );
			json.AddMember( rapidjson::StringRef( ATTR_FILE_PATH_POSY ),
							rapidjson::StringRef( texture->GetImagePath(2) ),
							_al );
			json.AddMember( rapidjson::StringRef( ATTR_FILE_PATH_NEGY ),
							rapidjson::StringRef( texture->GetImagePath(3) ),
							_al );
			json.AddMember( rapidjson::StringRef( ATTR_FILE_PATH_POSZ ),
							rapidjson::StringRef( texture->GetImagePath(4) ),
							_al );
			json.AddMember( rapidjson::StringRef( ATTR_FILE_PATH_NEGZ ),
							rapidjson::StringRef( texture->GetImagePath(5) ),
							_al );
			break;
	}

	return json;
}


void JsonSerializer_Texture::FromJson( rapidjson::Value& json, OUT void* object,
									   DeserFlag flag )
{
	Texture* texture = CAST_S( Texture*, object );

	bool emitEvents = texture->DoesEmitEvents();
	texture->SetEmitEvents( FALSE );


	if ( json.HasMember( ATTR_UNIQUE_ID ) ) {
		texture->SetUniqueID( json[ATTR_UNIQUE_ID].GetString() );
	}

	if ( flag == DeserFlag::UID_ONLY ) {
		return;
	}

	if ( json.HasMember( ATTR_TEXTURE_TYPE ) ) {
		FiniteString type( json[ATTR_TEXTURE_TYPE].GetString() );

		if ( type == TYPE_TEXTURE_2D ) {
			texture->SetTextureType( TextureType::TEXTURE_2D );
		}
		else if ( type == TYPE_CUBE_MAP ) {
			texture->SetTextureType( TextureType::CUBE_MAP );
		}
	}

	if ( json.HasMember( ATTR_LOAD_TYPE ) ) {
		FiniteString type( json[ATTR_LOAD_TYPE].GetString() );

		if ( type == TYPE_SCRIPTED ) {
			texture->SetLoadType( TextureLoadType::SCRIPTED );
		}
		else if ( type == TYPE_EXTERNAL ) {
			texture->SetLoadType( TextureLoadType::EXTERNAL );
		}
	}

	if ( json.HasMember( ATTR_HAS_MIPMAP ) ) {
		texture->SetHasMipmap( json[ATTR_HAS_MIPMAP].GetBool() );
	}

	if ( json.HasMember( ATTR_CLEAR_BUFFER ) ) {
		texture->SetClearBufferOnBind( json[ATTR_CLEAR_BUFFER].GetBool() );
	}

	switch ( texture->GetTextureType() ) {
		case TextureType::TEXTURE_2D:
			if ( json.HasMember( ATTR_FILE_PATH ) ) {
				texture->SetImagePath( 0, json[ATTR_FILE_PATH].GetString() );
			}
			break;

		case TextureType::CUBE_MAP:
			if ( json.HasMember( ATTR_FILE_PATH_POSX ) )	{
				texture->SetImagePath( 0, json[ATTR_FILE_PATH_POSX].GetString() );
			}
			if ( json.HasMember( ATTR_FILE_PATH_NEGX ) )	{
				texture->SetImagePath( 1, json[ATTR_FILE_PATH_NEGX].GetString() );
			}
			if ( json.HasMember( ATTR_FILE_PATH_POSY ) )	{
				texture->SetImagePath( 2, json[ATTR_FILE_PATH_POSY].GetString() );
			}
			if ( json.HasMember( ATTR_FILE_PATH_NEGY ) )	{
				texture->SetImagePath( 3, json[ATTR_FILE_PATH_NEGY].GetString() );
			}
			if ( json.HasMember( ATTR_FILE_PATH_POSZ ) )	{
				texture->SetImagePath( 4, json[ATTR_FILE_PATH_POSZ].GetString() );
			}
			if ( json.HasMember( ATTR_FILE_PATH_NEGZ ) )	{
				texture->SetImagePath( 5, json[ATTR_FILE_PATH_NEGZ].GetString() );
			}
			break;
	}


	texture->SetEmitEvents( emitEvents );
}
