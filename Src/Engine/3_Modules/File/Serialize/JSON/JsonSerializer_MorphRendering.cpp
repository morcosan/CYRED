// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_MorphRendering.h"
#include "../../../Render/Components/MorphRendering.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"
#include "../../../Render/Assets/Material.h"
#include "../../../Render/Assets/Morph.h"
#include "../../../Asset/AssetManager.h"
#include "../../../../2_BuildingBlocks/Random/Random.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_MorphRendering::ToJson( const void* object )
{
	const MorphRendering* morphRender = CAST_S( const MorphRendering*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( ATTR_ENABLED ),	
					morphRender->IsEnabled(),	
					_al );
	
	{
		Morph* morph = morphRender->GetMorph();
		if ( morph == NULL ) {
			rapidjson::Value nullNode;
			nullNode.SetNull();
			json.AddMember( rapidjson::StringRef( ATTR_MORPH ), nullNode, _al );
		}
		else {
			json.AddMember( rapidjson::StringRef( ATTR_MORPH ),
							rapidjson::StringRef( morph->GetUniqueID() ),
							_al );
		}
	}
	{
		Material* material = morphRender->GetMaterial();
		if ( material == NULL ) {
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

	json.AddMember( rapidjson::StringRef( ATTR_DUR_STATE ), 
					morphRender->GetDurationState(), 
					_al );
	json.AddMember( rapidjson::StringRef( ATTR_DUR_CHANGE ), 
					morphRender->GetDurationStateChange(), 
					_al );
	json.AddMember( rapidjson::StringRef( ATTR_IS_PLAYING ), 
					morphRender->IsPlaying(), 
					_al );

	return json;
}


void JsonSerializer_MorphRendering::FromJson( rapidjson::Value& json, OUT void* object, DeserFlag flag )
{
	MorphRendering* morphRender = CAST_S( MorphRendering*, object );

	bool emitEvents = morphRender->DoesEmitEvents();
	morphRender->SetEmitEvents( FALSE );


	if ( json.HasMember( ATTR_MORPH ) ) {
		if ( json[ATTR_MORPH].IsNull() ) {
			morphRender->SetMaterial( NULL );
		}
		else {
			cchar* uniqueID = json[ATTR_MORPH].GetString();
			Morph* morph = AssetManager::Singleton()->GetMorph( uniqueID );
			if ( morph == NULL ) {
				bool isOk = Random::ValidateUniqueID( uniqueID );
				if ( isOk )	{
					morph = new Morph();
					morph->SetUniqueID( uniqueID );
					AssetManager::Singleton()->AddMorph( morph );
				}
			}
			morphRender->SetMorph( morph );
		}
	}

	if ( json.HasMember( ATTR_MATERIAL ) ) {
		if ( json[ATTR_MATERIAL].IsNull() )	{
			morphRender->SetMaterial( NULL );
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
			morphRender->SetMaterial( material );
		}
	}

	if ( json.HasMember( ATTR_DUR_STATE ) ) {
		morphRender->SetDurationState( CAST_S( float, json[ATTR_DUR_STATE].GetDouble() ) );
	}

	if ( json.HasMember( ATTR_DUR_CHANGE ) ) {
		morphRender->SetDurationStateChange( CAST_S( float, json[ATTR_DUR_CHANGE].GetDouble() ) );
	}

	if ( json.HasMember( ATTR_IS_PLAYING ) ) {
		morphRender->SetIsPlaying( json[ATTR_IS_PLAYING].GetBool() );
	}


	morphRender->SetEmitEvents( emitEvents );
}
