// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/Data/DataArray.h"
#include "../../Render/OpenGL/Vertex.h"


namespace CYRED
{
	class DLL MeshLoader
	{
	private:
		MeshLoader() {}
		~MeshLoader() {}


	public:
		static Bool	LoadSingleMesh( const Char* buffer, UInt bufferSize, 
									DataArray<Vertex>& vertices, DataArray<UInt>& indices );

	};
}