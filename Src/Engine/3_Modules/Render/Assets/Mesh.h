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
		const char* GLOBAL_THIS	= "MESH";


	public:
		Mesh();
		virtual ~Mesh();


	public:
		void		LoadUniqueID	() override;
		void		LoadFullFile	() override;
		void		ClearAsset		() override;
		Asset*		Clone			() override;
		const char*	GetExtension	() override;


	public:
		//! must be called in order for mesh to work
		void BindToGPU	();

		MeshType		GetMeshType				() const;		
		MeshLoadType	GetLoadType				() const;		
		int			GetVBO					() const;		
		int			GetIBO					() const;			
		int			GetNumIndices			() const;	
		bool			DoesClearBuffersOnBind	() const;
		const char*		GetExternalPath			() const;

		void SetMeshType			( MeshType type );
		void SetLoadType			( MeshLoadType type );
		void SetVertices			( DataArray<Vertex>& vertices );
		void SetIndices				( DataArray<int>& indices );
		void SetClearBuffersOnBind	( bool value );
		void SetExternalPath		( const char* filePath );

		//script API
		void ClearVertices	();
		void AddVertex		( Vertex vertex );
		void ClearIndices	();
		void AddIndex		( int index );


	protected:
		uint				_vbo;
		uint				_ibo;
		int					_numIndices;
		MeshType			_meshType;
		bool				_clearBuffersOnBind;	//! when binding to GPU, clear image buffer
		
		MeshLoadType		_loadType;
		String				_externalPath;

		DataArray<Vertex>	_vertices;
		DataArray<int>		_indices;
	};
}