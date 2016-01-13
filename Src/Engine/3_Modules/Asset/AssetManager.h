// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"
#include "../../2_BuildingBlocks/String/String.h"


namespace CYRED
{
	class Mesh;
	class Material;
	class Shader;
	class Texture;
	class Scene;

	namespace Enum_StatusAssetAdd
	{
		enum Enum
		{
			SUCCESS
			, FAIL_INVALID_ID
			, FAIL_EXISTING
		};
	}

	typedef Enum_StatusAssetAdd::Enum StatusAssetAdd;
}


namespace CYRED
{
	ABSTRACT class DLL AssetManager
	{
		DECLARE_LOCAL_SINGLETON( AssetManager )


	public:
		virtual void Initialize	()	PURE_VIRTUAL;
		virtual void Finalize	()	PURE_VIRTUAL;

		virtual void ClearAll	()	PURE_VIRTUAL;

		virtual StatusAssetAdd	AddMesh		( Mesh* mesh )				PURE_VIRTUAL;
		virtual StatusAssetAdd	AddMaterial	( Material* material )		PURE_VIRTUAL;
		virtual StatusAssetAdd	AddShader	( Shader* shader )			PURE_VIRTUAL;
		virtual StatusAssetAdd	AddTexture	( Texture* texture )		PURE_VIRTUAL;
		virtual StatusAssetAdd	AddScene	( Scene* scene )			PURE_VIRTUAL;

		virtual Mesh*		GetMesh			( const Char* uniqueID )	PURE_VIRTUAL;
		virtual Material*	GetMaterial		( const Char* uniqueID )	PURE_VIRTUAL;
		virtual Shader*		GetShader		( const Char* uniqueID )	PURE_VIRTUAL;
		virtual Texture*	GetTexture		( const Char* uniqueID )	PURE_VIRTUAL;
		virtual Scene*		GetScene		( const Char* uniqueID )	PURE_VIRTUAL;

		virtual UInt		GetMaterialCount()				PURE_VIRTUAL;
		virtual Material*	GetMaterialAt	( UInt index )	PURE_VIRTUAL;

		virtual UInt		GetShaderCount	()				PURE_VIRTUAL;
		virtual Shader*		GetShaderAt		( UInt index )	PURE_VIRTUAL;

		virtual UInt		GetTextureCount	()				PURE_VIRTUAL;
		virtual Texture*	GetTextureAt	( UInt index )	PURE_VIRTUAL;
	};
}