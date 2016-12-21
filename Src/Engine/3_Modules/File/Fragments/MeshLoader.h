// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/Data/DataArray.h"
#include "../../../2_BuildingBlocks/String/String.h"
#include "../../Render/OpenGL/Vertex.h"


namespace CYRED
{
	class DLL MeshLoader
	{
		const Char* const HEADER			= "#CYRED Meshdata\n";
		const Char* const VERTICES_FORMAT	= "vertices %d\n";
		const Char* const INDICES_FORMAT	= "indices %d\n";
		const Char* const VERTEX_FORMAT		= "{pos:(%f,%f,%f) normal:(%f,%f,%f), uv:(%.2f,%.2f), color:(%.2f,%.2f,%.2f,%.2f)}\n";
		const Char* const INDEX_FORMAT		= "[%d,%d,%d]\n";



	public:
		MeshLoader() {}
		~MeshLoader() {}


	public:
		// import mesh from classic formats
		virtual Bool	ImportMesh( const Char* data, UInt dataSize, 
									OUT DataArray<Vertex>& vertices, 
									OUT DataArray<UInt>& indices )	{ return FALSE; };

		// load mesh from custom mesh file
		Bool	LoadMesh( const Char* data, OUT DataArray<Vertex>& vertices, 
						  OUT DataArray<UInt>& indices );
		// save mesh using custom format
		String	SaveMesh( DataArray<Vertex>& vertices, DataArray<UInt>& indices );
	};
}