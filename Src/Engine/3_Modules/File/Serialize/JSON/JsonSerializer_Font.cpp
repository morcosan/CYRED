// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Font.h"

#include "../../../Render/Assets/Font.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;


rapidjson::Value JsonSerializer_Font::ToJson( const void* object )
{
	const Font* font = CAST_S( const Font*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( ATTR_UNIQUE_ID ),
					rapidjson::StringRef( font->GetUniqueID() ),
					_al );

	json.AddMember( rapidjson::StringRef( ATTR_FILE_PATH ),
					rapidjson::StringRef( font->GetExternalPath() ),
					_al );

	json.AddMember( rapidjson::StringRef( ATTR_MAX_SIZE ),
					font->GetMaxSize(),
					_al );

	return json;
}


void JsonSerializer_Font::FromJson( rapidjson::Value& json, OUT void* object,
									DeserFlag flag )
{
	Font* font = CAST_S( Font*, object );

	bool emitEvents = font->DoesEmitEvents();
	font->SetEmitEvents( FALSE );


	if ( json.HasMember( ATTR_UNIQUE_ID ) ) {
		font->SetUniqueID( json[ATTR_UNIQUE_ID].GetString() );
	}

	if ( flag == DeserFlag::UID_ONLY ) {
		return;
	}

	if ( json.HasMember( ATTR_FILE_PATH ) ) {
		font->SetExternalPath( json[ATTR_FILE_PATH].GetString() );
	}

	if ( json.HasMember( ATTR_MAX_SIZE ) ) {
		font->SetMaxSize( json[ATTR_MAX_SIZE].GetInt() );
	}

	
	font->SetEmitEvents( emitEvents );
}
