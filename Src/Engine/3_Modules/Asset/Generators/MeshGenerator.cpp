// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "MeshGenerator.h"
#include "../../Render/Assets/Mesh.h"


using namespace CYRED;


void MeshGenerator::CreateCube( OUT Mesh* mesh )
{
	DataArray<Vertex>	vertices;
	DataArray<UInt>		indices;

	// front
	vertices.Add( Vertex( Vector3(-0.5f, -0.5f, 0.5f), Vector4(1,0,0,1), Vector3(0,0,1), Vector2(0, 1) ) );             
	vertices.Add( Vertex( Vector3(0.5f,	-0.5f,  0.5f), Vector4(0,1,0,1), Vector3(0,0,1), Vector2(1, 1) ) ); 
	vertices.Add( Vertex( Vector3(0.5f,  0.5f,  0.5f), Vector4(0,0,1,1), Vector3(0,0,1), Vector2(1, 0) ) ); 
	vertices.Add( Vertex( Vector3(-0.5f, 0.5f,  0.5f), Vector4(1,0,0,1), Vector3(0,0,1), Vector2(0, 0) ) );

	// back
	vertices.Add( Vertex( Vector3(-0.5f, -0.5f, -0.5f), Vector4(0,1,0,1), Vector3(0,0,-1), Vector2(1, 1) ) );             
	vertices.Add( Vertex( Vector3(-0.5f, 0.5f,  -0.5f), Vector4(0,0,1,1), Vector3(0,0,-1), Vector2(1, 0) ) ); 
	vertices.Add( Vertex( Vector3(0.5f,  0.5f,  -0.5f), Vector4(1,0,0,1), Vector3(0,0,-1), Vector2(0, 0) ) ); 
	vertices.Add( Vertex( Vector3(0.5f, -0.5f,  -0.5f), Vector4(0,1,0,1), Vector3(0,0,-1), Vector2(0, 1) ) );

	// top
	vertices.Add( Vertex( Vector3(-0.5f, 0.5f, -0.5f), Vector4(0,0,1,1), Vector3(0,1,0), Vector2(0, 0) ) );             
	vertices.Add( Vertex( Vector3(-0.5f, 0.5f,  0.5f), Vector4(1,0,0,1), Vector3(0,1,0), Vector2(0, 1) ) ); 
	vertices.Add( Vertex( Vector3( 0.5f, 0.5f,  0.5f), Vector4(0,1,0,1), Vector3(0,1,0), Vector2(1, 1) ) ); 
	vertices.Add( Vertex( Vector3( 0.5f, 0.5f, -0.5f), Vector4(0,0,1,1), Vector3(0,1,0), Vector2(1, 0) ) );

	// bottom
	vertices.Add( Vertex( Vector3(-0.5f, -0.5f, -0.5f), Vector4(1,0,0,1), Vector3(0,-1,0), Vector2(0, 1) ) );             
	vertices.Add( Vertex( Vector3( 0.5f, -0.5f, -0.5f), Vector4(0,1,0,1), Vector3(0,-1,0), Vector2(1, 1) ) ); 
	vertices.Add( Vertex( Vector3( 0.5f, -0.5f,  0.5f), Vector4(0,0,1,1), Vector3(0,-1,0), Vector2(1, 0) ) ); 
	vertices.Add( Vertex( Vector3(-0.5f, -0.5f,  0.5f), Vector4(1,0,0,1), Vector3(0,-1,0), Vector2(0, 0) ) );

	// right
	vertices.Add( Vertex( Vector3(0.5f, -0.5f, -0.5f), Vector4(0,1,0,1), Vector3(1,0,0), Vector2(1, 1) ) );             
	vertices.Add( Vertex( Vector3(0.5f,	 0.5f, -0.5f), Vector4(0,0,1,1), Vector3(1,0,0), Vector2(1, 0) ) ); 
	vertices.Add( Vertex( Vector3(0.5f,	 0.5f,  0.5f), Vector4(1,0,0,1), Vector3(1,0,0), Vector2(0, 0) ) ); 
	vertices.Add( Vertex( Vector3(0.5f, -0.5f,  0.5f), Vector4(0,1,0,1), Vector3(1,0,0), Vector2(0, 1) ) );

	// left
	vertices.Add( Vertex( Vector3(-0.5f, -0.5f, -0.5f), Vector4(0,0,1,1), Vector3(-1,0,0), Vector2(0, 1) ) );             
	vertices.Add( Vertex( Vector3(-0.5f, -0.5f,  0.5f), Vector4(1,0,0,1), Vector3(-1,0,0), Vector2(1, 1) ) ); 
	vertices.Add( Vertex( Vector3(-0.5f,  0.5f,  0.5f), Vector4(0,1,0,1), Vector3(-1,0,0), Vector2(1, 0) ) ); 
	vertices.Add( Vertex( Vector3(-0.5f,  0.5f, -0.5f), Vector4(0,0,1,1), Vector3(-1,0,0), Vector2(0, 0) ) );


	// front
	indices.Add(0); indices.Add(1); indices.Add(2);
	indices.Add(0); indices.Add(2); indices.Add(3);
	// back
	indices.Add(4); indices.Add(5); indices.Add(6);
	indices.Add(4); indices.Add(6); indices.Add(7);
	// top
	indices.Add(8); indices.Add(9); indices.Add(10);
	indices.Add(8); indices.Add(10); indices.Add(11);
	// bottom
	indices.Add(12); indices.Add(13); indices.Add(14);
	indices.Add(12); indices.Add(14); indices.Add(15);
	// right
	indices.Add(16); indices.Add(17); indices.Add(18);
	indices.Add(16); indices.Add(18); indices.Add(19);
	// left
	indices.Add(20); indices.Add(21); indices.Add(22);
	indices.Add(20); indices.Add(22); indices.Add(23);


	mesh->SetMeshType( MeshType::POLYGON );
	mesh->SetVertices( vertices );
	mesh->SetIndices( indices );
	mesh->BindToGPU();
}
