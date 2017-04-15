// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"
#include "AssetManager.h"
#include "../../2_BuildingBlocks/String/String.h"
#include "../../2_BuildingBlocks/Data/DataArray.h"
#include "../../2_BuildingBlocks/Data/DataMap.h"


namespace CYRED
{
	namespace NotAPI
	{
		class AssetManagerImpl : public AssetManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( AssetManagerImpl )


		public:
			void Initialize	()	override;
			void Finalize	()	override;

			void ClearAll	()	override;

			StatusAssetAdd	AddMesh		( Mesh* mesh )				override;
			StatusAssetAdd	AddMorph	( Morph* morph )			override;
			StatusAssetAdd	AddMaterial	( Material* material )		override;
			StatusAssetAdd	AddShader	( Shader* shader )			override;
			StatusAssetAdd	AddTexture	( Texture* texture )		override;
			StatusAssetAdd	AddScene	( Scene* scene )			override;
			StatusAssetAdd	AddScript	( Script* script )			override;
			StatusAssetAdd	AddPrefab	( Prefab* prefab )			override;

			void RemoveMesh		( Mesh* mesh )			override;
			void RemoveMorph	( Morph* morph )		override;
			void RemoveMaterial	( Material* material )	override;
			void RemoveShader	( Shader* shader )		override;
			void RemoveTexture	( Texture* texture )	override;
			void RemoveScene	( Scene* scene )		override;
			void RemoveScript	( Script* script )		override;
			void RemovePrefab	( Prefab* prefab )		override;

			Mesh*		GetMesh			( const Char* uniqueID )	override;
			Morph*		GetMorph		( const Char* uniqueID )	override;
			Material*	GetMaterial		( const Char* uniqueID )	override;
			Shader*		GetShader		( const Char* uniqueID )	override;
			Texture*	GetTexture		( const Char* uniqueID )	override;
			Scene*		GetScene		( const Char* uniqueID )	override;
			Script*		GetScript		( const Char* uniqueID )	override;
			Prefab*		GetPrefab		( const Char* uniqueID )	override;

			UInt		GetMaterialCount()							override;
			Material*	GetMaterialAt	( UInt index )				override;

			UInt		GetMeshCount	()							override;
			Mesh*		GetMeshAt		( UInt index )				override;

			UInt		GetMorphCount	()							override;
			Morph*		GetMorphAt		( UInt index )				override;

			UInt		GetShaderCount	()							override;
			Shader*		GetShaderAt		( UInt index )				override;

			UInt		GetTextureCount	()							override;
			Texture*	GetTextureAt	( UInt index )				override;

			UInt		GetSceneCount	()							override;
			Scene*		GetSceneAt		( UInt index )				override;

			UInt		GetScriptCount	()							override;
			Script*		GetScriptAt		( UInt index )				override;

			UInt		GetPrefabCount	()							override;
			Prefab*		GetPrefabAt		( UInt index )				override;


		protected:
			DataArray<Material*>	_materials;
			DataArray<Shader*>		_shaders;
			DataArray<Mesh*>		_meshes;
			DataArray<Morph*>		_morphs;
			DataArray<Texture*>		_textures;
			DataArray<Scene*>		_scenes;
			DataArray<Script*>		_scripts;
			DataArray<Prefab*>		_prefabs;
		};
	}
}