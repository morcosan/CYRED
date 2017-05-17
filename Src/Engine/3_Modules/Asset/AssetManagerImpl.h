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

			Mesh*		GetMesh			( const char* uniqueID )	override;
			Morph*		GetMorph		( const char* uniqueID )	override;
			Material*	GetMaterial		( const char* uniqueID )	override;
			Shader*		GetShader		( const char* uniqueID )	override;
			Texture*	GetTexture		( const char* uniqueID )	override;
			Scene*		GetScene		( const char* uniqueID )	override;
			Script*		GetScript		( const char* uniqueID )	override;
			Prefab*		GetPrefab		( const char* uniqueID )	override;

			int		GetMaterialCount()							override;
			Material*	GetMaterialAt	( int index )				override;

			int		GetMeshCount	()							override;
			Mesh*		GetMeshAt		( int index )				override;

			int		GetMorphCount	()							override;
			Morph*		GetMorphAt		( int index )				override;

			int		GetShaderCount	()							override;
			Shader*		GetShaderAt		( int index )				override;

			int		GetTextureCount	()							override;
			Texture*	GetTextureAt	( int index )				override;

			int		GetSceneCount	()							override;
			Scene*		GetSceneAt		( int index )				override;

			int		GetScriptCount	()							override;
			Script*		GetScriptAt		( int index )				override;

			int		GetPrefabCount	()							override;
			Prefab*		GetPrefabAt		( int index )				override;


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