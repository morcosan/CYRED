// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
		virtual ~MeshLoaderWindows() {}


	public:
		// import mesh from classic formats
		virtual bool	ImportMesh( const char* data, int dataSize, 
									OUT DataArray<Vertex>& vertices, 
									OUT DataArray<int>& indices )	override;
	};
}