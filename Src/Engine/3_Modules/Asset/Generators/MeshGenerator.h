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
	namespace NotAPI
	{
		class DLL MeshGenerator
		{
		public:
			MeshGenerator() {}
			virtual ~MeshGenerator() {}


		public:
			static void GenerateCube( OUT Mesh* mesh );
			static void GenerateQuad( OUT Mesh* mesh );
		};
	}
}