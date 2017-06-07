// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"
#include "../../2_BuildingBlocks/String/String.h"
#include "../Render/Assets/MeshEnum.h"

namespace CYRED
{
	class Mesh;
	class Morph;
	class Material;
	class Shader;
	class Texture;
	class Scene;
	class Script;
	class Prefab;
	class Font;

	namespace Enum_StatusAssetAdd
	{
		enum Enum
		{
			SUCCESS
			, FAIL_INVALID_ID
			, FAIL_EXISTING
		};
	}
	typedef Enum_StatusAssetAdd::Enum	StatusAssetAdd;
}


namespace CYRED
{
	ABSTRACT class DLL AssetManager
	{
		DECLARE_LOCAL_SINGLETON( AssetManager )


	public:
		virtual void Initialize	()	PURE_VIRTUAL;
		virtual void Finalize	()	PURE_VIRTUAL;
		/* it will not delete the assets, just reset them to defualt values */
		virtual void ClearAll	()	PURE_VIRTUAL;

		virtual StatusAssetAdd	AddMesh			( Mesh* asset )			PURE_VIRTUAL;
		virtual StatusAssetAdd	AddMorph		( Morph* asset )		PURE_VIRTUAL;
		virtual StatusAssetAdd	AddMaterial		( Material* asset )		PURE_VIRTUAL;
		virtual StatusAssetAdd	AddShader		( Shader* asset )		PURE_VIRTUAL;
		virtual StatusAssetAdd	AddTexture		( Texture* asset )		PURE_VIRTUAL;
		virtual StatusAssetAdd	AddScene		( Scene* asset )		PURE_VIRTUAL;
		virtual StatusAssetAdd	AddScript		( Script* asset )		PURE_VIRTUAL;
		virtual StatusAssetAdd	AddPrefab		( Prefab* asset )		PURE_VIRTUAL;
		virtual StatusAssetAdd	AddFont			( Font* asset )			PURE_VIRTUAL;

		virtual void			RemoveMesh		( Mesh* asset )			PURE_VIRTUAL;
		virtual void			RemoveMorph		( Morph* asset )		PURE_VIRTUAL;
		virtual void			RemoveMaterial	( Material* asset )		PURE_VIRTUAL;
		virtual void			RemoveShader	( Shader* asset )		PURE_VIRTUAL;
		virtual void			RemoveTexture	( Texture* asset )		PURE_VIRTUAL;
		virtual void			RemoveScene		( Scene* asset )		PURE_VIRTUAL;
		virtual void			RemoveScript	( Script* asset )		PURE_VIRTUAL;
		virtual void			RemovePrefab	( Prefab* asset )		PURE_VIRTUAL;
		virtual void			RemoveFont		( Font* asset )			PURE_VIRTUAL;

		virtual Mesh*			GetMesh			( cchar* uniqueID )		PURE_VIRTUAL;
		virtual Morph*			GetMorph		( cchar* uniqueID )		PURE_VIRTUAL;
		virtual Material*		GetMaterial		( cchar* uniqueID )		PURE_VIRTUAL;
		virtual Shader*			GetShader		( cchar* uniqueID )		PURE_VIRTUAL;
		virtual Texture*		GetTexture		( cchar* uniqueID )		PURE_VIRTUAL;
		virtual Scene*			GetScene		( cchar* uniqueID )		PURE_VIRTUAL;
		virtual Script*			GetScript		( cchar* uniqueID )		PURE_VIRTUAL;
		virtual Prefab*			GetPrefab		( cchar* uniqueID )		PURE_VIRTUAL;
		virtual Font*			GetFont			( cchar* uniqueID )		PURE_VIRTUAL;

		virtual int				GetMaterialCount()						PURE_VIRTUAL;
		virtual Material*		GetMaterialAt	( int index )			PURE_VIRTUAL;

		virtual int				GetMeshCount	()						PURE_VIRTUAL;
		virtual Mesh*			GetMeshAt		( int index )			PURE_VIRTUAL;

		virtual int				GetMorphCount	()						PURE_VIRTUAL;
		virtual Morph*			GetMorphAt		( int index )			PURE_VIRTUAL;
		
		virtual int				GetShaderCount	()						PURE_VIRTUAL;
		virtual Shader*			GetShaderAt		( int index )			PURE_VIRTUAL;
		
		virtual int				GetTextureCount	()						PURE_VIRTUAL;
		virtual Texture*		GetTextureAt	( int index )			PURE_VIRTUAL;
		
		virtual int				GetSceneCount	()						PURE_VIRTUAL;
		virtual Scene*			GetSceneAt		( int index )			PURE_VIRTUAL;
		
		virtual int				GetScriptCount	()						PURE_VIRTUAL;
		virtual Script*			GetScriptAt		( int index )			PURE_VIRTUAL;
		
		virtual int				GetPrefabCount	()						PURE_VIRTUAL;
		virtual Prefab*			GetPrefabAt		( int index )			PURE_VIRTUAL;

		virtual int				GetFontCount	()						PURE_VIRTUAL;
		virtual Font*			GetFontAt		( int index )			PURE_VIRTUAL;
	};
}