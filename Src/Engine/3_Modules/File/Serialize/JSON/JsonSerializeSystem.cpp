// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializeSystem.h"
#include "JsonSerializer_GameObject.h"
#include "JsonSerializer_Scene.h"
#include "JsonSerializer_Transform.h"
#include "JsonSerializer_ParticleEmitter.h"
#include "JsonSerializer_MeshRendering.h"
#include "JsonSerializer_MorphRendering.h"
#include "JsonSerializer_Material.h"
#include "JsonSerializer_Shader.h"
#include "JsonSerializer_Texture.h"
#include "JsonSerializer_Mesh.h"
#include "JsonSerializer_Morph.h"
#include "JsonSerializer_Camera.h"
#include "JsonSerializer_Light.h"
#include "JsonSerializer_AppConfig.h"
#include "JsonSerializer_Script.h"
#include "JsonSerializer_Scripter.h"
#include "JsonSerializer_Prefab.h"
#include "JsonSerializer_RigidBody.h"

#include "../../../../2_BuildingBlocks/GameObject.h"
#include "../../../../2_BuildingBlocks/Components/Transform.h"
#include "../../../Scene/Sections/Scene.h"
#include "../../../Render/Components/Camera.h"
#include "../../../Render/Components/Light.h"
#include "../../../Render/Components/ParticleEmitter.h"
#include "../../../Render/Components/MeshRendering.h"
#include "../../../Render/Components/MorphRendering.h"
#include "../../../Render/Assets/Material.h"
#include "../../../Render/Assets/Shader.h"
#include "../../../Render/Assets/Texture.h"
#include "../../../Render/Assets/Mesh.h"
#include "../../../Render/Assets/Morph.h"
#include "../../../Physics/Components/RigidBody.h"
#include "../../../../4_Application/AppConfig.h"
#include "../../../Script/Assets/Script.h"
#include "../../../Script/Components/Scripter.h"
#include "../../../Asset/Assets/Prefab.h"

#include "rapidjson\Include\document.h"
#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



JsonSerializeSystem::JsonSerializeSystem()
{
	AddSerializer<GameObject>		( new JsonSerializer_GameObject() );
	AddSerializer<Scene>			( new JsonSerializer_Scene() );

	AddSerializer<Transform>		( new JsonSerializer_Transform() );
	AddSerializer<Camera>			( new JsonSerializer_Camera() );
	AddSerializer<Light>			( new JsonSerializer_Light() );
	AddSerializer<ParticleEmitter>	( new JsonSerializer_ParticleEmitter() );
	AddSerializer<MeshRendering>	( new JsonSerializer_MeshRendering() );
	AddSerializer<MorphRendering>	( new JsonSerializer_MorphRendering() );
	AddSerializer<Scripter>			( new JsonSerializer_Scripter() );
	AddSerializer<RigidBody>		( new JsonSerializer_RigidBody() );

	AddSerializer<Material>			( new JsonSerializer_Material() );
	AddSerializer<Shader>			( new JsonSerializer_Shader() );
	AddSerializer<Texture>			( new JsonSerializer_Texture() );
	AddSerializer<Mesh>				( new JsonSerializer_Mesh() );
	AddSerializer<Morph>			( new JsonSerializer_Morph() );
	AddSerializer<Script>			( new JsonSerializer_Script() );
	AddSerializer<Prefab>			( new JsonSerializer_Prefab() );

	AddSerializer<AppConfig>		( new JsonSerializer_AppConfig() );
}


String JsonSerializeSystem::SerializeVec2( Vector2 value )
{
	JsonSerializer serializer;
	return serializer.SerializeVec2( value );
}


String JsonSerializeSystem::SerializeVec3( Vector3 value )
{
	JsonSerializer serializer;
	return serializer.SerializeVec3( value );
}


String JsonSerializeSystem::SerializeVec4( Vector4 value )
{
	JsonSerializer serializer;
	return serializer.SerializeVec4( value );
}


Vector2 JsonSerializeSystem::DeserializeVec2( cchar* data )
{
	JsonSerializer serializer;
	return serializer.DeserializeVec2( data );
}


Vector3 JsonSerializeSystem::DeserializeVec3( cchar * data )
{
	JsonSerializer serializer;
	return serializer.DeserializeVec3( data );
}


Vector4 JsonSerializeSystem::DeserializeVec4( cchar * data )
{
	JsonSerializer serializer;
	return serializer.DeserializeVec4( data );
}
