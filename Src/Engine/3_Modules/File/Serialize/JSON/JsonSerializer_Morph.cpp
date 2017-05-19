// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Morph.h"
#include "../../../Render/Assets/Morph.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_Morph::ToJson( const void* object )
{
	const Morph* morph = CAST_S( const Morph*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( UNIQUE_ID ),
					rapidjson::StringRef( morph->GetUniqueID() ),
					_al );

	// add file paths
	{
		rapidjson::Value arrayNode;
		arrayNode.SetArray();

		for ( int i = 0; i < morph->GetTotalStates(); ++i )
		{
			arrayNode.PushBack( rapidjson::StringRef( morph->GetFilePath( i ) ), _al );
		}

		json.AddMember( rapidjson::StringRef( FILE_PATHS ),
						arrayNode,
						_al );
	}

	return json;
}


void JsonSerializer_Morph::FromJson( rapidjson::Value& json, OUT void* object,
									 DeserFlag flag )
{
	Morph* morph = CAST_S( Morph*, object );

	bool emitEvents = morph->DoesEmitEvents();
	morph->SetEmitEvents( FALSE );

	if ( json.HasMember( UNIQUE_ID ) )
	{
		morph->SetUniqueID( json[UNIQUE_ID].GetString() );
	}

	if ( flag == DeserFlag::UID_ONLY )
	{
		return;
	}

	if ( json.HasMember( FILE_PATHS ) )
	{
		rapidjson::Value& paths = json[FILE_PATHS];

		morph->SetTotalStates( paths.Size() );

		for ( int i = 0; i < CAST_S(int, paths.Size()); ++i ) {
			morph->SetFilePath( i, paths[i].GetString() );
		}
	}
	
	morph->SetEmitEvents( emitEvents );
}
