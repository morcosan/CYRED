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
	Scripter* scripter = CAST_S( Scripter*, object );

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

					Iterator<String, DataUnion> iter = script->GetVarsListIterator();
					while ( iter.HasNext() ) {
						// create object
						rapidjson::Value objectNodeVar;
						objectNodeVar.SetObject();

						// add name
						objectNodeVar.AddMember( rapidjson::StringRef( VAR_NAME ),
												 rapidjson::StringRef( iter.GetKey().GetChar() ),
												 _al );

						switch ( iter.GetValue().GetValueType() )
						{
							case DataUnion::INT:
							{
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_TYPE ),
														 rapidjson::StringRef( VAR_TYPE_INT ),
														 _al );
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_VALUE ),
														 iter.GetValue().GetInt(),
														 _al );
								break;
							}

							case DataUnion::FLOAT:
							{
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_TYPE ),
														 rapidjson::StringRef( VAR_TYPE_FLOAT ),
														 _al );
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_VALUE ),
														 iter.GetValue().GetFloat(),
														 _al );
								break;
							}

							case DataUnion::BOOL:
							{
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_TYPE ),
														 rapidjson::StringRef( VAR_TYPE_BOOL ),
														 _al );
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_VALUE ),
														 iter.GetValue().GetBool(),
														 _al );
								break;
							}

							case DataUnion::STRING:
							{
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_TYPE ),
														 rapidjson::StringRef( VAR_TYPE_STRING ),
														 _al );
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_VALUE ),
														 rapidjson::StringRef( iter.GetValue().GetString() ),
														 _al );
								break;
							}

							case DataUnion::VECTOR2:
							{
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_TYPE ),
														 rapidjson::StringRef( VAR_TYPE_VEC2 ),
														 _al );
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_VALUE ),
														 _ToJsonVec2( iter.GetValue().GetVector2() ),
														 _al );
								break;
							}

							case DataUnion::VECTOR3:
							{
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_TYPE ),
														 rapidjson::StringRef( VAR_TYPE_VEC3 ),
														 _al );
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_VALUE ),
														 _ToJsonVec3( iter.GetValue().GetVector3() ),
														 _al );
								break;
							}

							case DataUnion::VECTOR4:
							{
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_TYPE ),
														 rapidjson::StringRef( VAR_TYPE_VEC4 ),
														 _al );
								objectNodeVar.AddMember( rapidjson::StringRef( VAR_VALUE ),
														 _ToJsonVec4( iter.GetValue().GetVector4() ),
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
	Scripter* scripter = CAST_S( Scripter*, object );

	Bool emitEvents = scripter->DoesEmitEvents();
	scripter->SetEmitEvents( FALSE );

	// load script
	if ( json.HasMember( SCRIPTS ) )
	{
		rapidjson::Value& scripts = json[SCRIPTS];

		for ( UInt i = 0; i < scripts.Size(); ++i ) {
			// load script uid
			if ( scripts[i].HasMember( SCRIPT_UID ) ) {
				scripter->SetScript( i, scripts[i][SCRIPT_UID].GetString() );
			}
			else {
				scripter->SetScript( i, "" );
			}
	
			// load vars
			if ( scripts[i].HasMember( SCRIPT_VARS ) ) {
				rapidjson::Value& vars = scripts[i][SCRIPT_VARS];
				Script* script = scripter->GetScript(i);

				if ( script != NULL ) {
					// parse script variables
					Iterator<String, DataUnion> iter = script->GetVarsListIterator();
					while ( iter.HasNext() ) {
						// set variable if exists
						for ( UInt j = 0; j < vars.Size(); j++ ) {
							if ( vars[j].HasMember( VAR_NAME ) 
								 && vars[j].HasMember( VAR_TYPE )
								 && vars[j].HasMember( VAR_VALUE )						
								 && iter.GetKey() == vars[j][VAR_NAME].GetString() ) 
							{
								DataUnion varValue;

								if ( vars[j][VAR_TYPE] == VAR_TYPE_INT 
									 && iter.GetValue().GetValueType() == DataUnion::INT ) 
								{
									varValue.SetInt( vars[j][VAR_VALUE].GetInt() );
									script->SetVariable( vars[j][VAR_NAME].GetString(), varValue );
								}
								else if ( vars[j][VAR_TYPE] == VAR_TYPE_FLOAT 
										  && iter.GetValue().GetValueType() == DataUnion::FLOAT ) 
								{
									varValue.SetFloat( vars[j][VAR_VALUE].GetDouble() );
									script->SetVariable( vars[j][VAR_NAME].GetString(), varValue );
								}
								else if ( vars[j][VAR_TYPE] == VAR_TYPE_BOOL 
										  && iter.GetValue().GetValueType() == DataUnion::BOOL ) 
								{
									varValue.SetBool( vars[j][VAR_VALUE].GetBool() );
									script->SetVariable( vars[j][VAR_NAME].GetString(), varValue );
								}
								else if ( vars[j][VAR_TYPE] == VAR_TYPE_STRING 
										  && iter.GetValue().GetValueType() == DataUnion::STRING ) 
								{
									varValue.SetString( vars[j][VAR_VALUE].GetString() );
									script->SetVariable( vars[j][VAR_NAME].GetString(), varValue );
								}
								else if ( vars[j][VAR_TYPE] == VAR_TYPE_VEC2 
										  && iter.GetValue().GetValueType() == DataUnion::VECTOR2 ) 
								{
									varValue.SetVector2( _FromJsonVec2( vars[j][VAR_VALUE] ) );
									script->SetVariable( vars[j][VAR_NAME].GetString(), varValue );
								}
								else if ( vars[j][VAR_TYPE] == VAR_TYPE_VEC3 
										  && iter.GetValue().GetValueType() == DataUnion::VECTOR3 )
								{
									varValue.SetVector3( _FromJsonVec3( vars[j][VAR_VALUE] ) );
									script->SetVariable( vars[j][VAR_NAME].GetString(), varValue );
								}
								else if ( vars[j][VAR_TYPE] == VAR_TYPE_VEC4 
										  && iter.GetValue().GetValueType() == DataUnion::VECTOR4 ) 
								{
									varValue.SetVector4( _FromJsonVec4( vars[j][VAR_VALUE] ) );
									script->SetVariable( vars[j][VAR_NAME].GetString(), varValue );
								}

								break;
							}
						}

						iter.Next();
					}
				}
			}
		}
	}

	scripter->SetEmitEvents( emitEvents );
}
