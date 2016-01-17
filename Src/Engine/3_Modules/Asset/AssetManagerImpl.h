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
			StatusAssetAdd	AddMaterial	( Material* material )		override;
			StatusAssetAdd	AddShader	( Shader* shader )			override;
			StatusAssetAdd	AddTexture	( Texture* texture )		override;
			StatusAssetAdd	AddScene	( Scene* scene )			override;

			Mesh*		GetMesh			( const Char* uniqueID )	override;
			Material*	GetMaterial		( const Char* uniqueID )	override;
			Shader*		GetShader		( const Char* uniqueID )	override;
			Texture*	GetTexture		( const Char* uniqueID )	override;
			Scene*		GetScene		( const Char* uniqueID )	override;

			UInt		GetMaterialCount()							override;
			Material*	GetMaterialAt	( UInt index )				override;

			UInt		GetShaderCount	()							override;
			Shader*		GetShaderAt		( UInt index )				override;

			UInt		GetTextureCount	()							override;
			Texture*	GetTextureAt	( UInt index )				override;


		protected:
			DataArray<Material*>	_materials;
			DataArray<Shader*>		_shaders;
			DataArray<Mesh*>		_meshes;
			DataArray<Texture*>		_textures;
			DataArray<Scene*>		_scenes;
		};
	}
}