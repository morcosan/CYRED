// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Texture.h"
#include "../../../Render/Assets/Texture.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_Texture::ToJson( void* object )
{
	Texture* texture = CAST_S( Texture*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( UNIQUE_ID ),
					rapidjson::StringRef( texture->GetUniqueID() ),
					_al );

	switch ( texture->GetTextureType() )
	{
		case TextureType::TEXTURE_2D:
			json.AddMember( rapidjson::StringRef( TYPE ),
							rapidjson::StringRef( TYPE_TEXTURE_2D ),
							_al );
			break;

		case TextureType::CUBE_MAP:
			json.AddMember( rapidjson::StringRef( TYPE ),
							rapidjson::StringRef( TYPE_CUBE_MAP ),
							_al );
			break;
	}

	json.AddMember( rapidjson::StringRef( HAS_MIPMAP ),
					texture->HasMipmap(),
					_al );
	json.AddMember( rapidjson::StringRef( CLEAR_BUFFER ),
					texture->DoesClearBufferOnBind(),
					_al );

	switch ( texture->GetTextureType() )
	{
		case TextureType::TEXTURE_2D:
			json.AddMember( rapidjson::StringRef( FILE_PATH ),
							rapidjson::StringRef( texture->GetImagePath(0) ),
							_al );
			break;

		case TextureType::CUBE_MAP:
			json.AddMember( rapidjson::StringRef( FILE_PATH_POSX ),
							rapidjson::StringRef( texture->GetImagePath(0) ),
							_al );
			json.AddMember( rapidjson::StringRef( FILE_PATH_NEGX ),
							rapidjson::StringRef( texture->GetImagePath(1) ),
							_al );
			json.AddMember( rapidjson::StringRef( FILE_PATH_POSY ),
							rapidjson::StringRef( texture->GetImagePath(2) ),
							_al );
			json.AddMember( rapidjson::StringRef( FILE_PATH_NEGY ),
							rapidjson::StringRef( texture->GetImagePath(3) ),
							_al );
			json.AddMember( rapidjson::StringRef( FILE_PATH_POSZ ),
							rapidjson::StringRef( texture->GetImagePath(4) ),
							_al );
			json.AddMember( rapidjson::StringRef( FILE_PATH_NEGZ ),
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

	Bool emitEvents = texture->DoesEmitEvents();
	texture->SetEmitEvents( FALSE );

	if ( json.HasMember( UNIQUE_ID ) )
	{
		texture->SetUniqueID( json[UNIQUE_ID].GetString() );
	}

	if ( flag == DeserFlag::UID_ONLY )
	{
		return;
	}

	if ( json.HasMember( TYPE ) )
	{
		FiniteString type( json[TYPE].GetString() );

		if ( type == TYPE_TEXTURE_2D )
		{
			texture->SetTextureType( TextureType::TEXTURE_2D );
		}
		if ( type == TYPE_CUBE_MAP )
		{
			texture->SetTextureType( TextureType::CUBE_MAP );
		}
	}
	if ( json.HasMember( HAS_MIPMAP ) )
	{
		texture->SetHasMipmap( json[HAS_MIPMAP].GetBool() );
	}
	if ( json.HasMember( CLEAR_BUFFER ) )
	{
		texture->SetClearBufferOnBind( json[CLEAR_BUFFER].GetBool() );
	}

	switch ( texture->GetTextureType() )
	{
		case TextureType::TEXTURE_2D:
		{
			if ( json.HasMember( FILE_PATH ) )
			{
				texture->SetImagePath( 0, json[FILE_PATH].GetString() );
				texture->SetLoadFromFile( TRUE );
			}
			break;
		}

		case TextureType::CUBE_MAP:
		{
			if ( json.HasMember( FILE_PATH_POSX ) )
			{
				texture->SetImagePath( 0, json[FILE_PATH_POSX].GetString() );
				texture->SetLoadFromFile( TRUE );
			}
			if ( json.HasMember( FILE_PATH_NEGX ) )
			{
				texture->SetImagePath( 1, json[FILE_PATH_NEGX].GetString() );
				texture->SetLoadFromFile( TRUE );
			}
			if ( json.HasMember( FILE_PATH_POSY ) )
			{
				texture->SetImagePath( 2, json[FILE_PATH_POSY].GetString() );
				texture->SetLoadFromFile( TRUE );
			}
			if ( json.HasMember( FILE_PATH_NEGY ) )
			{
				texture->SetImagePath( 3, json[FILE_PATH_NEGY].GetString() );
				texture->SetLoadFromFile( TRUE );
			}
			if ( json.HasMember( FILE_PATH_POSZ ) )
			{
				texture->SetImagePath( 4, json[FILE_PATH_POSZ].GetString() );
				texture->SetLoadFromFile( TRUE );
			}
			if ( json.HasMember( FILE_PATH_NEGZ ) )
			{
				texture->SetImagePath( 5, json[FILE_PATH_NEGZ].GetString() );
				texture->SetLoadFromFile( TRUE );
			}
			break;
		}
	}

	texture->SetEmitEvents( emitEvents );
}
