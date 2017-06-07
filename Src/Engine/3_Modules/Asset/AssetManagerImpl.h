// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"
#include "AssetManager.h"
#include "../../2_BuildingBlocks/String/String.h"
#include "../../2_BuildingBlocks/Data/DataArray.h"
#include "../../2_BuildingBlocks/Data/DataMap.h"


namespace CYRED
{
	namespace NonAPI
	{
		class AssetManagerImpl : public AssetManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( AssetManagerImpl )


		public:
			void			Initialize		()					override;
			void			Finalize		()					override;
	
			void			ClearAll		()					override;

			StatusAssetAdd	AddMesh			( Mesh* asset )		override;
			StatusAssetAdd	AddMorph		( Morph* asset )	override;
			StatusAssetAdd	AddMaterial		( Material* asset )	override;
			StatusAssetAdd	AddShader		( Shader* asset )	override;
			StatusAssetAdd	AddTexture		( Texture* asset )	override;
			StatusAssetAdd	AddScene		( Scene* asset )	override;
			StatusAssetAdd	AddScript		( Script* asset )	override;
			StatusAssetAdd	AddPrefab		( Prefab* asset )	override;
			StatusAssetAdd	AddFont			( Font* asset )		override;

			void			RemoveMesh		( Mesh* asset )		override;
			void			RemoveMorph		( Morph* asset )	override;
			void			RemoveMaterial	( Material* asset )	override;
			void			RemoveShader	( Shader* asset )	override;
			void			RemoveTexture	( Texture* asset )	override;
			void			RemoveScene		( Scene* asset )	override;
			void			RemoveScript	( Script* asset )	override;
			void			RemovePrefab	( Prefab* asset )	override;
			void			RemoveFont		( Font* asset )		override;

			Mesh*			GetMesh			( cchar* uniqueID )	override;
			Morph*			GetMorph		( cchar* uniqueID )	override;
			Material*		GetMaterial		( cchar* uniqueID )	override;
			Shader*			GetShader		( cchar* uniqueID )	override;
			Texture*		GetTexture		( cchar* uniqueID )	override;
			Scene*			GetScene		( cchar* uniqueID )	override;
			Script*			GetScript		( cchar* uniqueID )	override;
			Prefab*			GetPrefab		( cchar* uniqueID )	override;
			Font*			GetFont			( cchar* uniqueID )	override;

			int				GetMaterialCount()					override;
			Material*		GetMaterialAt	( int index )		override;

			int				GetMeshCount	()					override;
			Mesh*			GetMeshAt		( int index )		override;

			int				GetMorphCount	()					override;
			Morph*			GetMorphAt		( int index )		override;

			int				GetShaderCount	()					override;
			Shader*			GetShaderAt		( int index )		override;

			int				GetTextureCount	()					override;
			Texture*		GetTextureAt	( int index )		override;

			int				GetSceneCount	()					override;
			Scene*			GetSceneAt		( int index )		override;

			int				GetScriptCount	()					override;
			Script*			GetScriptAt		( int index )		override;

			int				GetPrefabCount	()					override;
			Prefab*			GetPrefabAt		( int index )		override;

			int				GetFontCount	()					override;
			Font*			GetFontAt		( int index )		override;


		private:
			DataArray<Material*>	_materials;
			DataArray<Shader*>		_shaders;
			DataArray<Mesh*>		_meshes;
			DataArray<Morph*>		_morphs;
			DataArray<Texture*>		_textures;
			DataArray<Scene*>		_scenes;
			DataArray<Script*>		_scripts;
			DataArray<Prefab*>		_prefabs;
			DataArray<Font*>		_fonts;
		};
	}
}