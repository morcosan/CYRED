// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/String/String.h"
#include "../../../2_BuildingBlocks/Data/DataArray.h"
#include "../../../2_BuildingBlocks/Asset.h"

#include "Shader.h"
#include "../OpenGL/Vertex.h"


namespace CYRED
{
	namespace Enum_MeshType
	{
		enum Enum
		{
			POLYGON
			, LINE
		};
	}

	namespace Enum_LoadType
	{
		enum Enum
		{
			EXTERNAL
			, GENERATED
		};
	}

	typedef Enum_MeshType::Enum	MeshType;
	typedef Enum_LoadType::Enum	LoadType;
}


namespace CYRED
{
	class DLL Mesh : public Asset
	{
	public:
		Mesh();
		virtual ~Mesh();


	public:
		void	LoadUniqueID	() override;
		void	LoadFullFile	() override;
		void	ClearAsset		() override;
		Asset*	Clone			() override;


	public:
		//! must be called in order for mesh to work
		void BindToGPU		();

		MeshType		GetMeshType				() const;		
		LoadType		GetLoadType				() const;		
		UInt			GetVBO					() const;		
		UInt			GetIBO					() const;			
		UInt			GetNumIndices			() const;	
		Bool			DoesClearBuffersOnBind	() const;
		const Char*		GetExternalPath			() const;

		void SetMeshType			( MeshType type );
		void SetLoadType			( LoadType type );
		void SetVertices			( DataArray<Vertex>& vertices );
		void SetIndices				( DataArray<UInt>& indices );
		void SetClearBuffersOnBind	( Bool value );
		void SetExternalPath		( const Char* filePath );


	protected:
		UInt				_vbo;
		UInt				_ibo;
		UInt				_numIndices;
		MeshType			_meshType;
		Bool				_clearBuffersOnBind;	//! when binding to GPU, clear image buffer
		
		LoadType			_loadType;
		String				_externalPath;

		DataArray<Vertex>	_vertices;
		DataArray<UInt>		_indices;
	};
}