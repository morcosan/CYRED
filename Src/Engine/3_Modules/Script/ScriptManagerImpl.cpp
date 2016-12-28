// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "ScriptManagerImpl.h"

#include "Assets\Script.h"
#include "../../2_BuildingBlocks/Math/Quaternion.h"
#include "../../2_BuildingBlocks/Math/Math.h"
#include "../../2_BuildingBlocks/Math/Vector2.h"
#include "../../2_BuildingBlocks/Math/Vector3.h"
#include "../../2_BuildingBlocks/Math/Vector4.h"
#include "../../2_BuildingBlocks/Math/Matrix4.h"
#include "../../2_BuildingBlocks/GameObject.h"
#include "../../2_BuildingBlocks/Component.h"
#include "../../2_BuildingBlocks/Components/Transform.h"
#include "../../2_BuildingBlocks/Asset.h"
#include "../../3_Modules/Render/RenderManager.h"
#include "../../3_Modules/Render/Components/Camera.h"
#include "../../3_Modules/Render/Components/MeshRendering.h"
#include "../../3_Modules/Render/Components/MorphRendering.h"
#include "../../3_Modules/Render/Components/ParticleEmitter.h"
#include "../../3_Modules/Render/Assets/Material.h"
#include "../../3_Modules/Render/Assets/Mesh.h"
#include "../../3_Modules/Render/Assets/Morph.h"
#include "../../3_Modules/Render/Assets/Shader.h"
#include "../../3_Modules/Render/Assets/Texture.h"
#include "../../3_Modules/File/FileManager.h"
#include "../../3_Modules/Input/InputManager.h"
#include "../../3_Modules/Asset/AssetManager.h"
#include "../../3_Modules/Event/EventManager.h"
#include "../../3_Modules/Scene/SceneManager.h"
#include "../../3_Modules/Time/TimeManager.h"
#include "../../3_Modules/Debug/DebugManager.h"

extern "C" 
{
	#include "Lua_523\Include\lua.h"
	#include "Lua_523\Include\lauxlib.h"
	#include "Lua_523\Include\lualib.h"
};

#include "LuaBridge\Include\LuaBridge.h"


using namespace CYRED;
using namespace NotAPI;


//! deferred definition
DEFINE_LOCAL_SINGLETON( ScriptManager, ScriptManagerImpl )
DEFINE_LOCAL_SINGLETON_IMPL( ScriptManagerImpl )



void ScriptManagerImpl::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = true;

	// create lua state
	_Lua = luaL_newstate();
	luaL_openlibs( _Lua );

	// register lua classes
	_RegisterGameObject();

	_RegisterComponent();
	_RegisterTransform();
	_RegisterCamera();
	_RegisterMeshRendering();
	_RegisterMorphRendering();
	_RegisterParticleEmitter();
	
	_RegisterFileManager();
	_RegisterInputManager();
	_RegisterRenderManager();
	_RegisterScriptManager();
	_RegisterAssetManager();
	_RegisterSceneManager();
	_RegisterDebugManager();
	_RegisterTimeManager();
	_RegisterEventManager();
	
	_RegisterAsset();
	_RegisterMaterial();
	_RegisterMesh();
	_RegisterMorph();
	_RegisterShader();
	_RegisterTexture();
	
	_RegisterMath();
	_RegisterVector2();
	_RegisterVector3();
	_RegisterVector4();
	_RegisterMatrix4();
	_RegisterQuaternion();
}


void ScriptManagerImpl::Finalize()
{
	ASSERT( _isInitialized );
}


lua_State* ScriptManagerImpl::GetLuaState() const
{
	return _Lua;
}


void ScriptManagerImpl::_RegisterGameObject()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<GameObject>( "GameObject" )
		.addProperty( "name",				&GameObject::GetName,	&GameObject::SetName )
		.addProperty( "enabled",			&GameObject::IsEnabled,	&GameObject::SetEnabled )
		.addFunction( "GetUniqueID",		&GameObject::GetUniqueID )
		.addFunction( "GetComponent",		&GameObject::GetComponentByName )
	.endClass();
}


void ScriptManagerImpl::_RegisterComponent()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<Component>( "Component" )
		.addFunction( "GetGameObject",	&Component::GetGameObject )
		.addProperty( "enabled",		&Component::IsEnabled,	&Component::SetEnabled )
	.endClass();
}


void ScriptManagerImpl::_RegisterTransform()
{
	luabridge::getGlobalNamespace( _Lua )
	.deriveClass<Transform, Component>( "Transform" )
		.addProperty( "positionWorld",		&Transform::GetPositionWorld,		&Transform::SetPositionWorld )
		.addProperty( "rotationWorld",		&Transform::GetRotationWorld,		&Transform::SetRotationWorld )
		.addProperty( "eulerRotationWorld",	&Transform::GetEulerRotationWorld,	&Transform::SetEulerRotationWorld )
		.addProperty( "scaleWorld",			&Transform::GetScaleWorld,			&Transform::SetScaleWorld )
		.addProperty( "positionLocal",		&Transform::GetPositionLocal,		&Transform::SetPositionLocal )
		.addProperty( "rotationLocal",		&Transform::GetRotationLocal,		&Transform::SetRotationLocal )
		.addProperty( "eulerRotationLocal",	&Transform::GetEulerRotationLocal,	&Transform::SetEulerRotationLocal )
		.addProperty( "scaleLocal",			&Transform::GetScaleLocal,			&Transform::SetScaleLocal )
		.addFunction( "GetLocalAxisX",		&Transform::GetLocalAxisX )
		.addFunction( "GetLocalAxisY",		&Transform::GetLocalAxisY )
		.addFunction( "GetLocalAxisZ",		&Transform::GetLocalAxisZ )
		.addFunction( "GetLocalForward",	&Transform::GetLocalForward )
		.addFunction( "TranslateByWorld",	&Transform::TranslateByWorld )
		.addFunction( "TranslateByLocal",	&Transform::TranslateByLocal )
		.addFunction( "Translate",			&Transform::Translate )
		.addFunction( "RotateByWorld",		&Transform::RotateByWorld )
		.addFunction( "RotateByLocal",		&Transform::RotateByLocal )
		//.addFunction( "Rotate",				&Transform::Rotate )
		.addFunction( "ScaleByConstant",	&Transform::ScaleByConstant )
	.endClass ();
}


void ScriptManagerImpl::_RegisterCamera()
{
	luabridge::getGlobalNamespace( _Lua )
		.deriveClass<Camera, Component>( "Camera" )
		.addProperty( "type",			&Camera::GetCameraTypeString,	&Camera::SetCameraTypeString )
		.addProperty( "fovYAngle",		&Camera::GetFovYAngle,			&Camera::SetFovYAngle )
		.addProperty( "nearClipping",	&Camera::GetNearClipping,		&Camera::SetNearClipping )
		.addProperty( "farClipping",	&Camera::GetFarClipping,		&Camera::SetFarClipping )
		.addProperty( "aspectRatio",	&Camera::GetAspectRatio,		&Camera::SetAspectRatio )
		.addProperty( "orthoSize",		&Camera::GetOrthoSize,			&Camera::SetOrthoSize )
		.endClass ();
}


void ScriptManagerImpl::_RegisterMeshRendering()
{
	luabridge::getGlobalNamespace( _Lua )
	.deriveClass<MeshRendering, Component>( "MeshRendering" )
		.addProperty( "material",	&MeshRendering::GetMaterial,	&MeshRendering::SetMaterial )
		.addProperty( "mesh",		&MeshRendering::GetMesh,		&MeshRendering::SetMesh )
	.endClass ();
}


void ScriptManagerImpl::_RegisterMorphRendering()
{
	luabridge::getGlobalNamespace( _Lua )
	.deriveClass<MorphRendering, Component>( "MorphRendering" )
	.endClass ();
}


void ScriptManagerImpl::_RegisterParticleEmitter()
{
	luabridge::getGlobalNamespace( _Lua )
	.deriveClass<ParticleEmitter, Component>( "ParticleEmitter" )
	.endClass ();
}


void ScriptManagerImpl::_RegisterFileManager()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<FileManager>( "FileManager" )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _Lua, FileManager::Singleton() );
	luabridge::setGlobal( _Lua, goRef, "FILE" );
}


void ScriptManagerImpl::_RegisterInputManager()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<InputManager>( "InputManager" )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _Lua, InputManager::Singleton() );
	luabridge::setGlobal( _Lua, goRef, "INPUT" );
}


void ScriptManagerImpl::_RegisterRenderManager()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<RenderManager>( "RenderManager" )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _Lua, RenderManager::Singleton() );
	luabridge::setGlobal( _Lua, goRef, "RENDER" );
}


void ScriptManagerImpl::_RegisterAssetManager()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<AssetManager>( "AssetManager" )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _Lua, AssetManager::Singleton() );
	luabridge::setGlobal( _Lua, goRef, "ASSET" );
}

void ScriptManagerImpl::_RegisterScriptManager()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<ScriptManager>( "ScriptManager" )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _Lua, ScriptManager::Singleton() );
	luabridge::setGlobal( _Lua, goRef, "SCRIPT" );
}


void ScriptManagerImpl::_RegisterSceneManager()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<SceneManager>( "SceneManager" )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _Lua, SceneManager::Singleton() );
	luabridge::setGlobal( _Lua, goRef, "SCENE" );
}


void ScriptManagerImpl::_RegisterDebugManager()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<DebugManager>( "DebugManager" )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _Lua, DebugManager::Singleton() );
	luabridge::setGlobal( _Lua, goRef, "DEBUG" );
}


void ScriptManagerImpl::_RegisterTimeManager()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<TimeManager>( "TimeManager" )
		.addFunction( "GetDeltaTime",		&TimeManager::GetDeltaTime )
		.addFunction( "GetRenderDeltaTime", &TimeManager::GetRenderDeltaTime )
		.addFunction( "GetDesiredFPS",		&TimeManager::GetDesiredFPS )
		.addFunction( "GetGameTime",		&TimeManager::GetGameTime )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _Lua, TimeManager::Singleton() );
	luabridge::setGlobal( _Lua, goRef, "TIME" );
}


void ScriptManagerImpl::_RegisterEventManager()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<EventManager>( "EventManager" )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _Lua, EventManager::Singleton() );
	luabridge::setGlobal( _Lua, goRef, "EVENT" );
}


void ScriptManagerImpl::_RegisterAsset()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<Asset>( "Asset" )
	.endClass();
}


void ScriptManagerImpl::_RegisterMaterial()
{
	luabridge::getGlobalNamespace( _Lua )
	.deriveClass<Material, Asset>( "Material" )
	.endClass ();
}


void ScriptManagerImpl::_RegisterMesh()
{
	luabridge::getGlobalNamespace( _Lua )
	.deriveClass<Mesh, Asset>( "Mesh" )
	.endClass ();
}


void ScriptManagerImpl::_RegisterMorph()
{
	luabridge::getGlobalNamespace( _Lua )
	.deriveClass<Morph, Asset>( "Morph" )
	.endClass ();
}


void ScriptManagerImpl::_RegisterShader()
{
	luabridge::getGlobalNamespace( _Lua )
	.deriveClass<Shader, Asset>( "Shader" )
	.endClass ();
}


void ScriptManagerImpl::_RegisterTexture()
{
	luabridge::getGlobalNamespace( _Lua )
	.deriveClass<Texture, Asset>( "Texture" )
	.endClass ();
}


void ScriptManagerImpl::_RegisterMath()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<Math>( "Math" )
	.endClass();
}


void ScriptManagerImpl::_RegisterVector2()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<Vector2>( "Vector2" )
		.addConstructor <void (*) (float, float)> ()
		.addData( "x",	&Vector2::x )
		.addData( "y",	&Vector2::y )
	.endClass();
}


void ScriptManagerImpl::_RegisterVector3()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<Vector3>( "Vector3" )
		.addConstructor <void (*) (float, float, float)> ()
		.addData( "x",	&Vector3::x )
		.addData( "y",	&Vector3::y )
		.addData( "z",	&Vector3::z )
	.endClass();
}


void ScriptManagerImpl::_RegisterVector4()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<Vector4>( "Vector4" )
		.addConstructor <void (*) (float, float, float, float)> ()
		.addData( "x",	&Vector4::x )
		.addData( "y",	&Vector4::y )
		.addData( "z",	&Vector4::z )
		.addData( "w",	&Vector4::w )
	.endClass();
}


void ScriptManagerImpl::_RegisterMatrix4()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<Matrix4>( "Matrix4" )
		.addConstructor <void (*) ()> ()
	.endClass();
}


void ScriptManagerImpl::_RegisterQuaternion()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass<Quaternion>( "Quaternion" )
		.addConstructor <void (*) ()> ()
	.endClass();
}
