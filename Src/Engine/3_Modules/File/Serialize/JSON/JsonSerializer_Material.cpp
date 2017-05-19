// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Material.h"
#include "../../../Render/Assets/Material.h"
#include "../../../Render/Assets/Shader.h"
#include "../../../Render/Assets/Texture.h"
#include "../../../Asset/AssetManager.h"
#include "../../../../2_BuildingBlocks/Random/Random.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_Material::ToJson( const void* object )
{
	const Material* material = CAST_S( const Material*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( UNIQUE_ID ),
					rapidjson::StringRef( material->GetUniqueID() ),
					_al );
	{
		Shader* shader = material->GetShader();
		if ( shader == NULL )
		{
			rapidjson::Value nullNode;
			nullNode.SetNull();
			json.AddMember( rapidjson::StringRef( SHADER ), nullNode, _al );
		}
		else
		{
			json.AddMember( rapidjson::StringRef( SHADER ),
							rapidjson::StringRef( shader->GetUniqueID() ),
							_al );
		}
	}
	json.AddMember( rapidjson::StringRef( WIREFRAME ),
					material->IsWireframe(),
					_al );
	json.AddMember( rapidjson::StringRef( LINE_WIDTH ),
					material->GetLineWidth(),
					_al );
	{
		FaceCulling faceCulling = material->GetFaceCulling();
		if ( faceCulling == FaceCulling::CULL_BACK )
		{
			json.AddMember( rapidjson::StringRef( CULL_FACE ),
							rapidjson::StringRef( CULL_FACE_BACK ),
							_al );
		}
		else if ( faceCulling == FaceCulling::CULL_FRONT )
		{
			json.AddMember( rapidjson::StringRef( CULL_FACE ),
							rapidjson::StringRef( CULL_FACE_FRONT ),
							_al );
		}
		else if ( faceCulling == FaceCulling::CULL_NONE )
		{
			json.AddMember( rapidjson::StringRef( CULL_FACE ),
							rapidjson::StringRef( CULL_FACE_NONE ),
							_al );
		}
	}
	{
		rapidjson::Value arrayNode;
		arrayNode.SetArray();

		int totalProperties = material->GetPropertiesCount();

		for ( int i = 0; i < totalProperties; ++i )
		{
			const char* name = material->GetPropertyNameAt( i );
			DataUnion& data = material->GetPropertyDataAt( i );

			rapidjson::Value objectNode;
			objectNode.SetObject();

			objectNode.AddMember( rapidjson::StringRef( PROP_NAME ),
								  rapidjson::StringRef( name ),
								  _al );

			switch ( data.GetValueType() )
			{
				case DataUnion::INT:
					objectNode.AddMember( rapidjson::StringRef( PROP_TYPE ),
										  rapidjson::StringRef( PROP_TYPE_INT ),
										  _al );
					objectNode.AddMember( rapidjson::StringRef( PROP_VALUE ),
										  data.GetInt(),
										  _al );
					break;

				case DataUnion::FLOAT:
					objectNode.AddMember( rapidjson::StringRef( PROP_TYPE ),
										  rapidjson::StringRef( PROP_TYPE_FLOAT ),
										  _al );
					objectNode.AddMember( rapidjson::StringRef( PROP_VALUE ),
										  data.GetFloat(),
										  _al );
					break;

				case DataUnion::VECTOR2:
					objectNode.AddMember( rapidjson::StringRef( PROP_TYPE ),
										  rapidjson::StringRef( PROP_TYPE_VEC2 ),
										  _al );
					objectNode.AddMember( rapidjson::StringRef( PROP_VALUE ),
										  _ToJsonVec2( data.GetVector2() ),
										  _al );
					break;

				case DataUnion::VECTOR3:
					objectNode.AddMember( rapidjson::StringRef( PROP_TYPE ),
										  rapidjson::StringRef( PROP_TYPE_VEC3 ),
										  _al );
					objectNode.AddMember( rapidjson::StringRef( PROP_VALUE ),
										  _ToJsonVec3( data.GetVector3() ),
										  _al );
					break;

				case DataUnion::VECTOR4:
					objectNode.AddMember( rapidjson::StringRef( PROP_TYPE ),
										  rapidjson::StringRef( PROP_TYPE_VEC4 ),
										  _al );
					objectNode.AddMember( rapidjson::StringRef( PROP_VALUE ),
										  _ToJsonVec4( data.GetVector4() ),
										  _al );
					break;

				case DataUnion::REFERENCE:
					{
						objectNode.AddMember( rapidjson::StringRef( PROP_TYPE ),
											  rapidjson::StringRef( PROP_TYPE_TEX ),
											  _al );

						Texture* texture = CAST_S( Texture*, data.GetReference() );
						if ( texture == NULL )
						{
							rapidjson::Value nullNode;
							nullNode.SetNull();
							objectNode.AddMember( rapidjson::StringRef( PROP_VALUE ), 
												  nullNode,
												  _al );
						}
						else
						{
							objectNode.AddMember( rapidjson::StringRef( PROP_VALUE ),
												  rapidjson::StringRef( texture->GetUniqueID() ),
												  _al );
						}
					}
					break;
			}

			arrayNode.PushBack( objectNode, _al );
		}

		json.AddMember( rapidjson::StringRef( PROPERTIES ),
						arrayNode,
						_al );
	}

	return json;
}


void JsonSerializer_Material::FromJson( rapidjson::Value& json, OUT void* object,
										DeserFlag flag )
{
	Material* material = CAST_S( Material*, object );
	AssetManager* assetMgr = AssetManager::Singleton();

	bool emitEvents = material->DoesEmitEvents();
	material->SetEmitEvents( FALSE );

	material->ClearProperties();

	if ( json.HasMember( UNIQUE_ID ) )
	{
		material->SetUniqueID( json[UNIQUE_ID].GetString() );
	}

	if ( flag == DeserFlag::UID_ONLY ) {
		return;
	}

	if ( json.HasMember( SHADER ) ) {
		if ( json[SHADER].IsNull() ) {
			material->SetShader( NULL );
		}
		else {
			const char* uniqueID = json[SHADER].GetString();
			Shader* shader = AssetManager::Singleton()->GetShader( uniqueID );
			if ( shader == NULL ) {
				bool isOk = Random::ValidateUniqueID( uniqueID );
				if ( isOk ) {
					shader = Memory::Alloc<Shader>();
					shader->SetUniqueID( uniqueID );
					AssetManager::Singleton()->AddShader( shader );
				}
				else {
					Memory::Free( shader );
					shader = NULL;
				}
			}
			material->SetShader( shader );
		}
	}
	if ( json.HasMember( WIREFRAME ) ) {
		material->SetWireframe( json[WIREFRAME].GetBool() );
	}
	if ( json.HasMember( LINE_WIDTH ) ) {
		material->SetLineWidth( CAST_S( float, json[LINE_WIDTH].GetDouble() ) );
	}
	if ( json.HasMember( CULL_FACE ) ) {
		String cullFace( json[CULL_FACE].GetString() );

		if ( cullFace == CULL_FACE_BACK ) {
			material->SetFaceCulling( FaceCulling::CULL_BACK );
		}
		else if ( cullFace == CULL_FACE_FRONT ) {
			material->SetFaceCulling( FaceCulling::CULL_FRONT );
		}
		else if ( cullFace == CULL_FACE_NONE ) {
			material->SetFaceCulling( FaceCulling::CULL_NONE );
		}
	}
	if ( json.HasMember( PROPERTIES ) ) {
		rapidjson::Value& properties = json[PROPERTIES];

		for ( int i = 0; i < CAST_S(int, properties.Size()); ++i ) {
			rapidjson::Value& property = properties[i];

			if ( property.HasMember( PROP_NAME ) &&
				 property.HasMember( PROP_TYPE ) &&
				 property.HasMember( PROP_VALUE ) )
			{
				String type( property[PROP_TYPE].GetString() );

				if ( type == PROP_TYPE_FLOAT ) {
					material->SetProperty( property[PROP_NAME].GetString(),
										   CAST_S( float, property[PROP_VALUE].GetDouble() ) );
				}
				else if ( type == PROP_TYPE_INT ) {
					material->SetProperty( property[PROP_NAME].GetString(),
										   property[PROP_VALUE].GetInt() );
				}
				else if ( type == PROP_TYPE_VEC2 ) {
					material->SetProperty( property[PROP_NAME].GetString(),
										   _FromJsonVec2( property[PROP_VALUE] ) );
				}
				else if ( type == PROP_TYPE_VEC3 ) {
					material->SetProperty( property[PROP_NAME].GetString(),
										   _FromJsonVec3( property[PROP_VALUE] ) );
				}
				else if ( type == PROP_TYPE_VEC4 ) {
					material->SetProperty( property[PROP_NAME].GetString(),
										   _FromJsonVec4( property[PROP_VALUE] ) );
				}
				else if ( type == PROP_TYPE_TEX ) {
					if ( property[PROP_VALUE].IsNull() ) {
						material->SetProperty( property[PROP_NAME].GetString(), NULL );
					}
					else {
						const char* uniqueID = property[PROP_VALUE].GetString();
						Texture* texture = AssetManager::Singleton()->GetTexture( uniqueID );
						if ( texture == NULL ) {
							bool isOk = Random::ValidateUniqueID( uniqueID );
							if ( isOk )	{
								texture = Memory::Alloc<Texture>();
								texture->SetUniqueID( uniqueID );
								AssetManager::Singleton()->AddTexture( texture );
							}
							else {
								Memory::Free( texture );
								texture = NULL;
							}
						}
						material->SetProperty( property[PROP_NAME].GetString(), texture );
					}
				}
			}
		}
	}

	material->SetEmitEvents( emitEvents );
}
