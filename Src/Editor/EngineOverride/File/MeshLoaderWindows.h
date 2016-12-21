// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "CyredModule_File.h"


namespace CYRED
{
	class MeshLoaderWindows : public MeshLoader
	{
	public:
		MeshLoaderWindows() {}
		~MeshLoaderWindows() {}


	public:
		// import mesh from classic formats
		virtual Bool	ImportMesh( const Char* data, UInt dataSize, 
									OUT DataArray<Vertex>& vertices, 
									OUT DataArray<UInt>& indices )	override;
	};
}