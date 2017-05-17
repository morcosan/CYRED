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
		const char* const HEADER			= "#CYRED Meshdata\n";


	public:
		MeshLoader() {}
		virtual ~MeshLoader() {}


	public:
		// import mesh from classic formats
		virtual bool	ImportMesh( const char* data, int dataSize, 
									OUT DataArray<Vertex>& vertices, 
									OUT DataArray<int>& indices )	{ return FALSE; };

		// load mesh from custom mesh file
		virtual bool	LoadMesh( const char* data, OUT DataArray<Vertex>& vertices, 
								  OUT DataArray<int>& indices );
		// save mesh using custom format
		virtual String	SaveMesh( DataArray<Vertex>& vertices, DataArray<int>& indices );
	};
}