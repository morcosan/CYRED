// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "ScriptManagerImpl.h"

#include "Assets\Script.h"
#include "../../2_BuildingBlocks/Math/Quaternion.h"
#include "../../2_BuildingBlocks/Math/Math.h"
#include "../../2_BuildingBlocks/Math/Vector2.h"
#include "../../2_BuildingBlocks/Math/Vector3.h"
#include "../../2_BuildingBlocks/Math/Vector4.h"
#include "../../2_BuildingBlocks/Math/Matrix4.h"
#include "../../2_BuildingBlocks/Random/Random.h"
#include "../../2_BuildingBlocks/String/FiniteString.h"
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
#include "../../3_Modules/Asset/Assets/Prefab.h"
#include "../../3_Modules/File/FileManager.h"
#include "../../3_Modules/Input/InputManager.h"
#include "../../3_Modules/Asset/AssetManager.h"
#include "../../3_Modules/Event/EventManager.h"
#include "../../3_Modules/Scene/SceneManager.h"
#include "../../3_Modules/Time/TimeManager.h"
#include "../../3_Modules/Debug/DebugManager.h"
#include "../../4_Application/Application.h"

extern "C" 
{
	#include "Lua_523\Include\lua.h"
	#include "Lua_523\Include\lauxlib.h"
	#include "Lua_523\Include\lualib.h"
};

#include "LuaBridge\Include\LuaBridge.h"


using namespace CYRED;
using namespace NonAPI;


//! deferred definition
DEFINE_LOCAL_SINGLETON( ScriptManager, ScriptManagerImpl )
DEFINE_LOCAL_SINGLETON_IMPL( ScriptManagerImpl )


int ScriptManagerImpl::LuaFunc_Print( lua_State* L )
{
	// redirect lua print function to debug manager
	int nargs = lua_gettop( L );
	for ( int i = 1; i <= nargs; ++i ) {
		int argType = lua_type( L, i );
		switch ( argType ) {
			case LUA_TNIL:
				DebugManager::Singleton()->Log( "nil" );
				break;

			case LUA_TSTRING:
				DebugManager::Singleton()->Log( lua_tostring( L, i ) );
				break;

			case LUA_TBOOLEAN:
				DebugManager::Singleton()->Log( lua_toboolean( L, i ) ? "TRUE" : "FALSE" );
				break;

			case LUA_TNUMBER:
				DebugManager::Singleton()->LogFloat( CAST_S( float, lua_tonumber( L, i ) ) );
				break;

			case LUA_TLIGHTUSERDATA:
				DebugManager::Singleton()->Log( "Lua light userdata." );
				break;

			case LUA_TTABLE:
				DebugManager::Singleton()->Log( "Lua table." );
				break;

			case LUA_TFUNCTION:
				DebugManager::Singleton()->Log( "Lua function." );
				break;

			case LUA_TUSERDATA:
				DebugManager::Singleton()->Log( "Lua userdata." );
				break;
				
			default:
				DebugManager::Singleton()->Log( "Unknown Lua type." );
				break;
		}
	}

	return 0;
}


void ScriptManagerImpl::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = true;

	// create lua state
	_L = luaL_newstate();
	luaL_openlibs( _L );

	// override some lua functions
	_OverrideLuaFunc();

	// register lua classes
	_RegisterApplication();

	_RegisterNode();
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
	_RegisterPrefab();
	
	_RegisterRandom();
	_RegisterMath();
	_RegisterVector2();
	_RegisterVector3();
	_RegisterVector4();
	_RegisterMatrix4();
	_RegisterQuaternion();
	_RegisterVertex();
}


void ScriptManagerImpl::Finalize()
{
	ASSERT( _isInitialized );

	lua_close( _L );
}


lua_State* ScriptManagerImpl::GetLuaState() const
{
	return _L;
}


void ScriptManagerImpl::_OverrideLuaFunc()
{
	// redirect print function
	struct luaL_Reg printlib[] = {
		{ "print", ScriptManagerImpl::LuaFunc_Print },
		{ NULL, NULL } /* end of array */
	};

	lua_getglobal( _L, "_G" );
	luaL_setfuncs( _L, printlib, 0 );
	lua_pop( _L, 1);
}


void ScriptManagerImpl::_RegisterApplication()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<Application>( "Application" )
		.addFunction( "Exit", &Application::Exit )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _L, Application::Singleton() );
	luabridge::setGlobal( _L, goRef, "APP" );
}


void ScriptManagerImpl::_RegisterNode()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<Node>( "Node" )
		.addProperty( "parentNode",				&Node::GetParentNode,	&Node::SetParentNode )
		.addProperty( "childNodeCount",			&Node::GetChildNodeCount )
	
		.addFunction( "AddChildNode",			&Node::AddChildNode )
		.addFunction( "InsertChildNode",		&Node::InsertChildNode )
		.addFunction( "GetChildNodeAt",			&Node::GetChildNodeAt )
		.addFunction( "DeleteAllChildNodes",	&Node::DeleteAllChildNodes )
	.endClass();
}


void ScriptManagerImpl::_RegisterGameObject()
{
	luabridge::getGlobalNamespace( _L )
	.deriveClass<GameObject, Node>( "GameObject" )
		.addProperty( "name",				&GameObject::GetName,	&GameObject::SetName )
		.addProperty( "tag",				&GameObject::GetTag,	&GameObject::SetTag )
		.addProperty( "enabled",			&GameObject::IsEnabled,	&GameObject::SetEnabled )
		.addFunction( "GetUniqueID",		&GameObject::GetUniqueID )

		.addFunction( "AddComponent_Transform",			&GameObject::AddComponent<Transform> )
		.addFunction( "AddComponent_Camera",			&GameObject::AddComponent<Camera> )
		.addFunction( "AddComponent_MeshRendering",		&GameObject::AddComponent<MeshRendering> )
		.addFunction( "AddComponent_MorphRendering",	&GameObject::AddComponent<MorphRendering> )
		.addFunction( "AddComponent_ParticleEmitter",	&GameObject::AddComponent<ParticleEmitter> )

		.addFunction( "GetComponent_Transform",			&GameObject::GetComponent<Transform> )
		.addFunction( "GetComponent_Camera",			&GameObject::GetComponent<Camera> )
		.addFunction( "GetComponent_MeshRendering",		&GameObject::GetComponent<MeshRendering> )
		.addFunction( "GetComponent_MorphRendering",	&GameObject::GetComponent<MorphRendering> )
		.addFunction( "GetComponent_ParticleEmitter",	&GameObject::GetComponent<ParticleEmitter> )
	.endClass();
}


void ScriptManagerImpl::_RegisterComponent()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<Component>( "Component" )
		.addFunction( "GetGameObject",	&Component::GetGameObject )
		.addProperty( "enabled",		&Component::IsEnabled,	&Component::SetEnabled )
	.endClass();
}


void ScriptManagerImpl::_RegisterTransform()
{
	luabridge::getGlobalNamespace( _L )
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
		.addFunction( "RotateByWorld",		&Transform::RotateByWorld )
		.addFunction( "RotateByLocal",		&Transform::RotateByLocal )
		.addFunction( "ScaleWithAdd",	&Transform::ScaleWithAdd )
	.endClass ();
}


void ScriptManagerImpl::_RegisterCamera()
{
	luabridge::getGlobalNamespace( _L )
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
	luabridge::getGlobalNamespace( _L )
	.deriveClass<MeshRendering, Component>( "MeshRendering" )
		.addProperty( "material",	&MeshRendering::GetMaterial,	&MeshRendering::SetMaterial )
		.addProperty( "mesh",		&MeshRendering::GetMesh,		&MeshRendering::SetMesh )
	.endClass ();
}


void ScriptManagerImpl::_RegisterMorphRendering()
{
	luabridge::getGlobalNamespace( _L )
	.deriveClass<MorphRendering, Component>( "MorphRendering" )
	.endClass ();
}


void ScriptManagerImpl::_RegisterParticleEmitter()
{
	luabridge::getGlobalNamespace( _L )
	.deriveClass<ParticleEmitter, Component>( "ParticleEmitter" )
	.endClass ();
}


void ScriptManagerImpl::_RegisterFileManager()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<FileManager>( "FileManager" )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _L, FileManager::Singleton() );
	luabridge::setGlobal( _L, goRef, "FILE" );
}


void ScriptManagerImpl::_RegisterInputManager()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<InputManager>( "InputManager" )
		.addFunction( "KeyDown",				&InputManager::KeyDown )
		.addFunction( "KeyDownFirstTime",		&InputManager::KeyDownFirstTime )
		.addFunction( "KeyUp",					&InputManager::KeyUp )
		.addFunction( "KeyUpFirstTime",			&InputManager::KeyUpFirstTime )
		.addFunction( "MousePosition",			&InputManager::MousePosition )
		.addFunction( "MouseDeltaPosition",		&InputManager::MouseDeltaPosition )
		.addFunction( "ScrollWheel",			&InputManager::ScrollWheel )
	.endClass();

	// bind keycodes as globals
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::A) ),				"KEY_A" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::B) ),				"KEY_B" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::C) ),				"KEY_C" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::D) ),				"KEY_D" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::E) ),				"KEY_E" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::F) ),				"KEY_F" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::G) ),				"KEY_G" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::H) ),				"KEY_H" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::I) ),				"KEY_I" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::J) ),				"KEY_J" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::K) ),				"KEY_K" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::L) ),				"KEY_L" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::M) ),				"KEY_M" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::N) ),				"KEY_N" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::O) ),				"KEY_O" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::P) ),				"KEY_P" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::Q) ),				"KEY_Q" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::R) ),				"KEY_R" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::S) ),				"KEY_S" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::T) ),				"KEY_T" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::V) ),				"KEY_V" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::W) ),				"KEY_W" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::X) ),				"KEY_X" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::Y) ),				"KEY_Y" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::Z) ),				"KEY_Z" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::NUM_0) ),			"KEY_0" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::NUM_1) ),			"KEY_1" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::NUM_2) ),			"KEY_2" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::NUM_3) ),			"KEY_3" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::NUM_4) ),			"KEY_4" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::NUM_5) ),			"KEY_5" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::NUM_6) ),			"KEY_6" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::NUM_7) ),			"KEY_7" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::NUM_8) ),			"KEY_8" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::NUM_9) ),			"KEY_9" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::ALT) ),				"KEY_ALT" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::CTRL) ),			"KEY_CTRL" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::SHIFT) ),			"KEY_SHIFT" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::ARROW_UP) ),		"KEY_ARROW_UP" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::ARROW_DOWN) ),		"KEY_ARROW_DOWN" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::ARROW_LEFT) ),		"KEY_ARROW_LEFT" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::ARROW_RIGHT) ),		"KEY_ARROW_RIGHT" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::MOUSE_LEFT) ),		"KEY_MOUSE_LEFT" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::MOUSE_MIDDLE) ),	"KEY_MOUSE_MIDDLE" );
	luabridge::setGlobal( _L, luabridge::LuaRef( _L, (int)(KeyCode::MOUSE_RIGHT) ),		"KEY_MOUSE_RIGHT" );

	// set global
	luabridge::LuaRef goRef( _L, InputManager::Singleton() );
	luabridge::setGlobal( _L, goRef, "INPUT" );
}


void ScriptManagerImpl::_RegisterRenderManager()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<RenderManager>( "RenderManager" )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _L, RenderManager::Singleton() );
	luabridge::setGlobal( _L, goRef, "RENDER" );
}


void ScriptManagerImpl::_RegisterAssetManager()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<AssetManager>( "AssetManager" )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _L, AssetManager::Singleton() );
	luabridge::setGlobal( _L, goRef, "ASSET" );
}

void ScriptManagerImpl::_RegisterScriptManager()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<ScriptManager>( "ScriptManager" )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _L, ScriptManager::Singleton() );
	luabridge::setGlobal( _L, goRef, "SCRIPT" );
}


void ScriptManagerImpl::_RegisterSceneManager()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<SceneManager>( "SceneManager" )
		.addFunction( "FindGameObject",	&SceneManager::FindGameObject )
		.addFunction( "Instantiate",	&SceneManager::Instantiate )
		.addFunction( "Destroy",		&SceneManager::Destroy )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _L, SceneManager::Singleton() );
	luabridge::setGlobal( _L, goRef, "SCENE" );
}


void ScriptManagerImpl::_RegisterDebugManager()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<DebugManager>( "DebugManager" )
		.addFunction( "Log",		&DebugManager::Log )
		.addFunction( "LogInt",		&DebugManager::LogInt )
		.addFunction( "LogFloat",	&DebugManager::LogFloat )
		.addFunction( "LogVec2",	&DebugManager::LogVec2 )
		.addFunction( "LogVec3",	&DebugManager::LogVec3 )
		.addFunction( "LogVec4",	&DebugManager::LogVec4 )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _L, DebugManager::Singleton() );
	luabridge::setGlobal( _L, goRef, "DEBUG" );
}


void ScriptManagerImpl::_RegisterTimeManager()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<TimeManager>( "TimeManager" )
		.addFunction( "GetDeltaTime",		&TimeManager::GetDeltaTime )
		.addFunction( "GetRenderDeltaTime", &TimeManager::GetRenderDeltaTime )
		.addFunction( "GetDesiredFPS",		&TimeManager::GetDesiredFPS )
		.addFunction( "GetGameTime",		&TimeManager::GetGameTime )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _L, TimeManager::Singleton() );
	luabridge::setGlobal( _L, goRef, "TIME" );
}


void ScriptManagerImpl::_RegisterEventManager()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<EventManager>( "EventManager" )
	.endClass();

	// set global
	luabridge::LuaRef goRef( _L, EventManager::Singleton() );
	luabridge::setGlobal( _L, goRef, "EVENT" );
}


void ScriptManagerImpl::_RegisterAsset()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<Asset>( "Asset" )
	.endClass();
}


void ScriptManagerImpl::_RegisterMaterial()
{
	luabridge::getGlobalNamespace( _L )
	.deriveClass<Material, Asset>( "Material" )
	.endClass ();
}


void ScriptManagerImpl::_RegisterMesh()
{
	luabridge::getGlobalNamespace( _L )
	.deriveClass<Mesh, Asset>( "Mesh" )
		.addFunction( "ClearVertices",	&Mesh::ClearVertices )
		.addFunction( "AddVertex",		&Mesh::AddVertex )
		.addFunction( "ClearIndices",	&Mesh::ClearIndices )
		.addFunction( "AddIndex",		&Mesh::AddIndex )
	.endClass ();
}


void ScriptManagerImpl::_RegisterMorph()
{
	luabridge::getGlobalNamespace( _L )
	.deriveClass<Morph, Asset>( "Morph" )
	.endClass ();
}


void ScriptManagerImpl::_RegisterShader()
{
	luabridge::getGlobalNamespace( _L )
	.deriveClass<Shader, Asset>( "Shader" )
	.endClass ();
}


void ScriptManagerImpl::_RegisterTexture()
{
	luabridge::getGlobalNamespace( _L )
	.deriveClass<Texture, Asset>( "Texture" )
		.addFunction( "SetImageData",	&Texture::SetImageData )
		.addFunction( "SetPixel",		&Texture::SetPixel )
	.endClass ();
}


void ScriptManagerImpl::_RegisterPrefab()
{
	luabridge::getGlobalNamespace( _L )
	.deriveClass<Prefab, Asset>( "Prefab" )
	.endClass ();
}


void ScriptManagerImpl::_RegisterRandom()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<Random>( "Random" )
		.addStaticFunction( "FromRangeFloat",	&Random::FromRangeFloat )
		.addStaticFunction( "FromRangeInt",		&Random::FromRangeInt )
		.addStaticFunction( "FromCircle",		&Random::FromCircle )
		.addStaticFunction( "FromCircleEdge",	&Random::FromCircleEdge )
		.addStaticFunction( "FromSphere",		&Random::FromSphere )
		.addStaticFunction( "FromSphereEdge",	&Random::FromSphereEdge )
	.endClass();
}


void ScriptManagerImpl::_RegisterMath()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<Math>( "Math" )
	.endClass();
}


void ScriptManagerImpl::_RegisterVector2()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<Vector2>( "Vector2" )
		.addConstructor <void (*) (float, float)> ()
		.addData( "x",	&Vector2::x )
		.addData( "y",	&Vector2::y )
	.endClass();
}


void ScriptManagerImpl::_RegisterVector3()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<Vector3>( "Vector3" )
		.addConstructor <void (*) (float, float, float)> ()
		.addData( "x",	&Vector3::x )
		.addData( "y",	&Vector3::y )
		.addData( "z",	&Vector3::z )
	.endClass();
}


void ScriptManagerImpl::_RegisterVector4()
{
	luabridge::getGlobalNamespace( _L )
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
	luabridge::getGlobalNamespace( _L )
	.beginClass<Matrix4>( "Matrix4" )
		.addConstructor <void (*) ()> ()
	.endClass();
}


void ScriptManagerImpl::_RegisterQuaternion()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<Quaternion>( "Quaternion" )
		.addConstructor <void (*) ()> ()
	.endClass();
}


void ScriptManagerImpl::_RegisterVertex()
{
	luabridge::getGlobalNamespace( _L )
	.beginClass<Vertex>( "Vertex" )
		.addConstructor <void (*) (Vector3, Vector4, Vector3, Vector2, Vector3, Vector3)> ()
	.endClass();
}
