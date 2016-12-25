// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Scripter.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"
#include "../../../Asset/AssetManager.h"
#include "../../../../2_BuildingBlocks/Random/Random.h"
#include "../../../Script/Assets/Script.h"
#include "../../../Script/Components/Scripter.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_Scripter::ToJson( void* object )
{
	COMP::Scripter* scripter = CAST_S( COMP::Scripter*, object );

	rapidjson::Value json;
	json.SetObject();

	// add enabled
	json.AddMember( rapidjson::StringRef( ENABLED ),	
					scripter->IsEnabled(),	
					_al );
	
	// add scripts
	{
		// create list
		rapidjson::Value arrayNodeScripts;
		arrayNodeScripts.SetArray();

		for ( UInt i = 0; i < scripter->GetScriptsCount(); ++i ) {
			Script* script = scripter->GetScript( i );

			// create object
			rapidjson::Value objectNode;
			objectNode.SetObject();

			if ( script == NULL ) {
				objectNode.AddMember( rapidjson::StringRef( SCRIPT_UID ), "", _al );
			}
			else {
				// add uid
				objectNode.AddMember( rapidjson::StringRef( SCRIPT_UID ),
									  rapidjson::StringRef( script->GetUniqueID() ),
									  _al );

				// add variables
				{
					// create list
					rapidjson::Value arrayNodeVars;
					arrayNodeVars.SetArray();

					Iterator<String, Script::LuaVar> iter = script->GetVarsListIterator();
					while ( iter.HasNext() ) {
						// create object
						rapidjson::Value objectNodeVar;
						objectNodeVar.SetObject();

						// add name
						String var = iter.GetKey();
						const Char* varName = var.GetChar();
						objectNodeVar.AddMember( rapidjson::StringRef( VAR_NAME ),
												 rapidjson::StringRef( varName ),
												 _al );

						switch ( iter.GetValue().type )
						{
							case DataUnion::INT:
							{
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_TYPE ),
														 rapidjson::StringRef( VAR_TYPE_INT ),
														 _al );
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_VALUE ),
														 iter.GetValue().value.GetInt(),
														 _al );
								break;
							}

							case DataUnion::FLOAT:
							{
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_TYPE ),
														 rapidjson::StringRef( VAR_TYPE_FLOAT ),
														 _al );
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_VALUE ),
														 iter.GetValue().value.GetFloat(),
														 _al );
								break;
							}

							case DataUnion::BOOL:
							{
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_TYPE ),
														 rapidjson::StringRef( VAR_TYPE_BOOL ),
														 _al );
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_VALUE ),
														 iter.GetValue().value.GetBool(),
														 _al );
								break;
							}

							case DataUnion::STRING:
							{
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_TYPE ),
														 rapidjson::StringRef( VAR_TYPE_STRING ),
														 _al );
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_VALUE ),
														 rapidjson::StringRef( iter.GetValue().value.GetString() ),
														 _al );
								break;
							}

							case DataUnion::VECTOR2:
							{
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_TYPE ),
														 rapidjson::StringRef( VAR_TYPE_VEC2 ),
														 _al );
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_VALUE ),
														 _ToJsonVec2( iter.GetValue().value.GetVector2() ),
														 _al );
								break;
							}

							case DataUnion::VECTOR3:
							{
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_TYPE ),
														 rapidjson::StringRef( VAR_TYPE_VEC3 ),
														 _al );
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_VALUE ),
														 _ToJsonVec3( iter.GetValue().value.GetVector3() ),
														 _al );
								break;
							}

							case DataUnion::VECTOR4:
							{
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_TYPE ),
														 rapidjson::StringRef( VAR_TYPE_VEC4 ),
														 _al );
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_VALUE ),
														 _ToJsonVec4( iter.GetValue().value.GetVector4() ),
														 _al );
								break;
							}
						}

						arrayNodeVars.PushBack( objectNodeVar, _al );

						iter.Next();
					}

					objectNode.AddMember( rapidjson::StringRef( SCRIPT_VARS ),
										  arrayNodeVars,
										  _al );
				}
			}

			arrayNodeScripts.PushBack( objectNode, _al );
		}

		// add to json
		json.AddMember( rapidjson::StringRef( SCRIPTS ),
						arrayNodeScripts,
						_al );
	}

	return json;
}


void JsonSerializer_Scripter::FromJson( rapidjson::Value& json, OUT void* object, 
										 DeserFlag flag )
{
	COMP::Scripter* scripter = CAST_S( COMP::Scripter*, object );

	Bool emitEvents = scripter->DoesEmitEvents();
	scripter->SetEmitEvents( FALSE );

	// load script
	//if ( json.HasMember( SCRIPT ) )
	//{
	//	if ( json[SCRIPT].IsNull() )
	//	{
	//		scripter->SetScript( NULL );
	//	}
	//	else
	//	{
	//		const Char* uniqueID = json[SCRIPT].GetString();
	//		Script* script = AssetManager::Singleton()->GetScript( uniqueID );
	//		if ( script == NULL ) {
	//			Bool isOk = Random::ValidateUniqueID( uniqueID );
	//			if ( isOk )	{
	//				script = Memory::Alloc<Script>();
	//				script->SetUniqueID( uniqueID );
	//				AssetManager::Singleton()->AddScript( script );
	//			}
	//		}
	//		scripter->SetScript( script );
	//	}
	//}

	scripter->SetEmitEvents( emitEvents );
}
