// Copyright (c) 2015 Morco (www.morco.ro)
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

		virtual StatusAssetAdd	AddMesh		( Mesh* mesh )				PURE_VIRTUAL;
		virtual StatusAssetAdd	AddMorph	( Morph* morph )			PURE_VIRTUAL;
		virtual StatusAssetAdd	AddMaterial	( Material* material )		PURE_VIRTUAL;
		virtual StatusAssetAdd	AddShader	( Shader* shader )			PURE_VIRTUAL;
		virtual StatusAssetAdd	AddTexture	( Texture* texture )		PURE_VIRTUAL;
		virtual StatusAssetAdd	AddScene	( Scene* scene )			PURE_VIRTUAL;
		virtual StatusAssetAdd	AddScript	( Script* script )			PURE_VIRTUAL;
		virtual StatusAssetAdd	AddPrefab	( Prefab* prefab )			PURE_VIRTUAL;

		virtual void RemoveMesh		( Mesh* mesh )			PURE_VIRTUAL;
		virtual void RemoveMorph	( Morph* morph )		PURE_VIRTUAL;
		virtual void RemoveMaterial	( Material* material )	PURE_VIRTUAL;
		virtual void RemoveShader	( Shader* shader )		PURE_VIRTUAL;
		virtual void RemoveTexture	( Texture* texture )	PURE_VIRTUAL;
		virtual void RemoveScene	( Scene* scene )		PURE_VIRTUAL;
		virtual void RemoveScript	( Script* script )		PURE_VIRTUAL;
		virtual void RemovePrefab	( Prefab* prefab )		PURE_VIRTUAL;

		virtual Mesh*		GetMesh			( const char* uniqueID )	PURE_VIRTUAL;
		virtual Morph*		GetMorph		( const char* uniqueID )	PURE_VIRTUAL;
		virtual Material*	GetMaterial		( const char* uniqueID )	PURE_VIRTUAL;
		virtual Shader*		GetShader		( const char* uniqueID )	PURE_VIRTUAL;
		virtual Texture*	GetTexture		( const char* uniqueID )	PURE_VIRTUAL;
		virtual Scene*		GetScene		( const char* uniqueID )	PURE_VIRTUAL;
		virtual Script*		GetScript		( const char* uniqueID )	PURE_VIRTUAL;
		virtual Prefab*		GetPrefab		( const char* uniqueID )	PURE_VIRTUAL;

		virtual int		GetMaterialCount()				PURE_VIRTUAL;
		virtual Material*	GetMaterialAt	( int index )	PURE_VIRTUAL;

		virtual int		GetMeshCount	()				PURE_VIRTUAL;
		virtual Mesh*		GetMeshAt		( int index )	PURE_VIRTUAL;

		virtual int		GetMorphCount	()				PURE_VIRTUAL;
		virtual Morph*		GetMorphAt		( int index )	PURE_VIRTUAL;

		virtual int		GetShaderCount	()				PURE_VIRTUAL;
		virtual Shader*		GetShaderAt		( int index )	PURE_VIRTUAL;

		virtual int		GetTextureCount	()				PURE_VIRTUAL;
		virtual Texture*	GetTextureAt	( int index )	PURE_VIRTUAL;

		virtual int		GetSceneCount	()				PURE_VIRTUAL;
		virtual Scene*		GetSceneAt		( int index )	PURE_VIRTUAL;

		virtual int		GetScriptCount	()				PURE_VIRTUAL;
		virtual Script*		GetScriptAt		( int index )	PURE_VIRTUAL;

		virtual int		GetPrefabCount	()				PURE_VIRTUAL;
		virtual Prefab*		GetPrefabAt		( int index )	PURE_VIRTUAL;
	};
}