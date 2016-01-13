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

	typedef Enum_MeshType::Enum	MeshType;
}


namespace CYRED
{
	class DLL Mesh : public Asset
	{
	public:
		Mesh();
		virtual ~Mesh();


	public:
		void LoadUniqueID	() override;
		void LoadFullFile	() override;


	public:
		//! must be called in order for mesh to work
		void BindToGPU		();

		MeshType		GetMeshType				() const;		
		UInt			GetVBO					() const;		
		UInt			GetIBO					() const;			
		UInt			GetNumIndices			() const;	
		Bool			DoesClearBuffersOnBind	() const;

		void SetMeshType			( MeshType type );
		void SetVertices			( DataArray<Vertex>& vertices );
		void SetIndices				( DataArray<UInt>& indices );
		void SetClearBuffersOnBind	( Bool value );


	protected:
		UInt				_vbo;
		UInt				_ibo;
		UInt				_numIndices;
		MeshType			_meshType;
		Bool				_clearBuffersOnBind;	//! when binding to GPU, clear image buffer

		DataArray<Vertex>	_vertices;
		DataArray<UInt>		_indices;
	};
}