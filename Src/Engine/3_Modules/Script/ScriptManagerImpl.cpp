// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "ScriptManagerImpl.h"
#include "Assets\Script.h"
#include "../Debug/DebugManager.h"

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


void ScriptManagerImpl::LoadLuaScript( Script* script, const Char* data )
{
	// load script data
	switch ( luaL_loadstring( _Lua, data ) )
	{
		case LUA_ERRSYNTAX:
		{
			// syntax error
			DebugManager::Singleton()->Log( lua_tostring( _Lua, -1 ) );
			break;
		}
			
		case LUA_OK:
		{
			// script loaded
			// run lua
			switch ( lua_pcall( _Lua, 0, 0, 0 ) ) {
				case LUA_ERRRUN:
				{
					// run error
					DebugManager::Singleton()->Log( lua_tostring( _Lua, -1 ) );
					break;
				}

				case 0:
				{
					// run lua: success
					// load default functions
					
					// OnStart
					{
						// load from data
						luabridge::LuaRef luaFunc = luabridge::getGlobal( _Lua, "OnStart" );
						if ( !luaFunc.isNil() ) {
							// add to script
							script->SetLuaData( "OnStart", Memory::Alloc<luabridge::LuaRef>( luaFunc ) );
						}
					}
					
					// OnUpdate
					{
						// load from data
						luabridge::LuaRef luaFunc = luabridge::getGlobal( _Lua, "OnUpdate" );
						if ( !luaFunc.isNil() ) {
							// add to script
							script->SetLuaData( "OnUpdate", Memory::Alloc<luabridge::LuaRef>( luaFunc ) );
						}
					}

					break;
				}
			}
		}
	}
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
}


void ScriptManagerImpl::_RegisterVector3()
{
}


void ScriptManagerImpl::_RegisterVector4()
{
}


void ScriptManagerImpl::_RegisterMatrix4()
{
}


void ScriptManagerImpl::_RegisterQuaternion()
{
}
