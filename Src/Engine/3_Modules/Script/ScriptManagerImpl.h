// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"
#include "ScriptManager.h"



namespace CYRED
{
	namespace NotAPI
	{
		class ScriptManagerImpl : public ScriptManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( ScriptManagerImpl )


		public:
			void Initialize	()	override;
			void Finalize	()	override;

			lua_State* GetLuaState() const override;


		protected:
			lua_State*	_Lua;

			void _RegisterGameObject();

			void _RegisterComponent();
			void _RegisterTransform();
			void _RegisterCamera();
			void _RegisterMeshRendering();
			void _RegisterMorphRendering();
			void _RegisterParticleEmitter();

			void _RegisterFileManager();
			void _RegisterInputManager();
			void _RegisterRenderManager();
			void _RegisterAssetManager();
			void _RegisterScriptManager();
			void _RegisterSceneManager();
			void _RegisterDebugManager();
			void _RegisterTimeManager();
			void _RegisterEventManager();

			void _RegisterAsset();
			void _RegisterMaterial();
			void _RegisterMesh();
			void _RegisterMorph();
			void _RegisterShader();
			void _RegisterTexture();

			void _RegisterMath();
			void _RegisterVector2();
			void _RegisterVector3();
			void _RegisterVector4();
			void _RegisterMatrix4();
			void _RegisterQuaternion();
		};
	}
}