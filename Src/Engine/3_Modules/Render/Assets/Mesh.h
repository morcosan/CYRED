// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/String/String.h"
#include "../../../2_BuildingBlocks/Data/DataArray.h"
#include "../../../2_BuildingBlocks/Asset.h"

#include "Shader.h"
#include "../OpenGL/Vertex.h"
#include "MeshEnum.h"


namespace CYRED
{
	class DLL Mesh : public Asset
	{
		const Char* GLOBAL_THIS	= "MESH";


	public:
		Mesh();
		virtual ~Mesh();


	public:
		void		LoadUniqueID	() override;
		void		LoadFullFile	() override;
		void		ClearAsset		() override;
		Asset*		Clone			() override;
		const Char*	GetExtension	() override;


	public:
		//! must be called in order for mesh to work
		void BindToGPU	();

		MeshType		GetMeshType				() const;		
		MeshLoadType	GetLoadType				() const;		
		UInt			GetVBO					() const;		
		UInt			GetIBO					() const;			
		UInt			GetNumIndices			() const;	
		Bool			DoesClearBuffersOnBind	() const;
		const Char*		GetExternalPath			() const;

		void SetMeshType			( MeshType type );
		void SetLoadType			( MeshLoadType type );
		void SetVertices			( DataArray<Vertex>& vertices );
		void SetIndices				( DataArray<UInt>& indices );
		void SetClearBuffersOnBind	( Bool value );
		void SetExternalPath		( const Char* filePath );

		void ClearVertices	();
		void AddVertex		( Vertex vertex );
		void ClearIndices	();
		void AddIndex		( UInt index );


	protected:
		UInt				_vbo;
		UInt				_ibo;
		UInt				_numIndices;
		MeshType			_meshType;
		Bool				_clearBuffersOnBind;	//! when binding to GPU, clear image buffer
		
		MeshLoadType		_loadType;
		String				_externalPath;

		DataArray<Vertex>	_vertices;
		DataArray<UInt>		_indices;
	};
}