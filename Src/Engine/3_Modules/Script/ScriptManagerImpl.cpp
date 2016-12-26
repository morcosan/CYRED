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
	_RegisterComponent();
	_RegisterCamera();
	_RegisterTransform();
	_RegisterMeshRenderer();
	_RegisterMorphRenderer();
	_RegisterParticleEmitter();
	
	_RegisterFileManager();
	_RegisterInputManager();
	_RegisterRenderManager();
	_RegisterAssetManager();
	_RegisterSceneManager();
	_RegisterDebugManager();
	_RegisterTimeManager();
	
	_RegisterGameObject();
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


void ScriptManagerImpl::_RegisterComponent()
{
}


void ScriptManagerImpl::_RegisterCamera()
{
}


void ScriptManagerImpl::_RegisterTransform()
{
}


void ScriptManagerImpl::_RegisterMeshRenderer()
{
}


void ScriptManagerImpl::_RegisterMorphRenderer()
{
}


void ScriptManagerImpl::_RegisterParticleEmitter()
{
}


void ScriptManagerImpl::_RegisterFileManager()
{
}


void ScriptManagerImpl::_RegisterInputManager()
{
}


void ScriptManagerImpl::_RegisterRenderManager()
{
}


void ScriptManagerImpl::_RegisterAssetManager()
{
}


void ScriptManagerImpl::_RegisterSceneManager()
{
}


void ScriptManagerImpl::_RegisterDebugManager()
{
}


void ScriptManagerImpl::_RegisterTimeManager()
{
}


void ScriptManagerImpl::_RegisterGameObject()
{
}


void ScriptManagerImpl::_RegisterMaterial()
{
}


void ScriptManagerImpl::_RegisterMesh()
{
}


void ScriptManagerImpl::_RegisterMorph()
{
}


void ScriptManagerImpl::_RegisterShader()
{
}


void ScriptManagerImpl::_RegisterTexture()
{
}


void ScriptManagerImpl::_RegisterMath()
{
}


void ScriptManagerImpl::_RegisterVector2()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass <Vector2> ("Vector2")
		.addConstructor <void (*) (float, float)> ()
		.addProperty( "x",	&Vector2::S_GetX, &Vector2::S_SetX )
		.addProperty( "y",	&Vector2::S_GetY, &Vector2::S_SetY )
	.endClass();
}


void ScriptManagerImpl::_RegisterVector3()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass <Vector3> ("Vector3")
		.addConstructor <void (*) (float, float, float)> ()
		.addProperty( "x",	&Vector3::S_GetX, &Vector3::S_SetX )
		.addProperty( "y",	&Vector3::S_GetY, &Vector3::S_SetY )
		.addProperty( "z",	&Vector3::S_GetZ, &Vector3::S_SetZ )
	.endClass();
}


void ScriptManagerImpl::_RegisterVector4()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass <Vector4> ("Vector4")
		.addConstructor <void (*) (float, float, float, float)> ()
		.addProperty( "x",	&Vector4::S_GetX, &Vector4::S_SetX )
		.addProperty( "y",	&Vector4::S_GetY, &Vector4::S_SetY )
		.addProperty( "z",	&Vector4::S_GetZ, &Vector4::S_SetZ )
		.addProperty( "w",	&Vector4::S_GetW, &Vector4::S_SetW )
	.endClass();
}


void ScriptManagerImpl::_RegisterMatrix4()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass <Matrix4> ("Matrix4")
		.addConstructor <void (*) ()> ()
	.endClass();
}


void ScriptManagerImpl::_RegisterQuaternion()
{
	luabridge::getGlobalNamespace( _Lua )
	.beginClass <Quaternion> ("Quaternion")
		.addConstructor <void (*) ()> ()
	.endClass();
}
