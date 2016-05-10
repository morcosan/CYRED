// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"


namespace CYRED
{
	class Mesh;
}


namespace CYRED
{
	class DLL MeshGenerator
	{
	private:
		MeshGenerator() {}
		~MeshGenerator() {}


	public:
		static void CreateCube( OUT Mesh* mesh );
	};
}