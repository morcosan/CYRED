// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "MeshLoader.h"
#include "../FileManager.h"
#include "../../../2_BuildingBlocks/String/FiniteString.h"

#include <string>
#include <sstream>


using namespace CYRED;


bool MeshLoader::LoadMesh( const char* data, OUT DataArray<Vertex>& vertices, 
						   OUT DataArray<int>& indices )
{
	int headerIndex = strncmp( data, HEADER, strlen( HEADER ) );
	if ( headerIndex != 0 ) {
		return FALSE;
	}

	// parse data and get values
	char* dataPtr = NO_CONST( char*, data );
	// jump over header
	dataPtr += strlen( HEADER );

	// read vertices
	int countVertices = strtol( dataPtr, &dataPtr, 10 );
	for ( int i = 0; i < countVertices; i++ ) {
		// read position
		Vector3 pos;
		pos.x = strtof( dataPtr, &dataPtr );
		pos.y = strtof( dataPtr, &dataPtr );
		pos.z = strtof( dataPtr, &dataPtr );

		// read color
		Vector4 color;
		color.x = strtof( dataPtr, &dataPtr );
		color.y = strtof( dataPtr, &dataPtr );
		color.z = strtof( dataPtr, &dataPtr );
		color.w = strtof( dataPtr, &dataPtr );

		// read normal
		Vector3 normal;
		normal.x = strtof( dataPtr, &dataPtr );
		normal.y = strtof( dataPtr, &dataPtr );
		normal.z = strtof( dataPtr, &dataPtr );

		// read uv
		Vector2 uv;
		uv.x = strtof( dataPtr, &dataPtr );
		uv.y = strtof( dataPtr, &dataPtr );

		// read tangent
		Vector3 tan;
		tan.x = strtof( dataPtr, &dataPtr );
		tan.y = strtof( dataPtr, &dataPtr );
		tan.z = strtof( dataPtr, &dataPtr );

		// read bitangent
		Vector3 bitan;
		bitan.x = strtof( dataPtr, &dataPtr );
		bitan.y = strtof( dataPtr, &dataPtr );
		bitan.z = strtof( dataPtr, &dataPtr );
		

		// create vertex
		vertices.Add( Vertex( pos, color, normal, uv, tan, bitan ) );
	}

	// read indeces
	int countIndices = strtol( dataPtr, &dataPtr, 10 );
	for ( int i = 0; i < countIndices; i++ ) {
		indices.Add( strtol( dataPtr, &dataPtr, 10 ) );
	}

	return TRUE;
}


String MeshLoader::SaveMesh( DataArray<Vertex>& vertices, DataArray<int>& indices )
{
	// create string
	std::ostringstream dataStream;

	// add header
	dataStream << HEADER;

	// add vertices
	dataStream << '\n' << vertices.Size() << '\n';
	// populate array
	for ( int i = 0; i < vertices.Size(); i++ ) {
		dataStream  << vertices[i].position.x << ' '
					<< vertices[i].position.y << ' '
					<< vertices[i].position.z << ' '
					<< vertices[i].color.x << ' '
					<< vertices[i].color.y << ' '
					<< vertices[i].color.z << ' '
					<< vertices[i].color.w << ' '
					<< vertices[i].normal.x << ' '
					<< vertices[i].normal.y << ' '
					<< vertices[i].normal.z << ' '
					<< vertices[i].uv.x << ' '
					<< vertices[i].uv.y << ' '
					<< vertices[i].tangent.x << ' '
					<< vertices[i].tangent.y << ' '
					<< vertices[i].tangent.z << ' '
					<< vertices[i].bitangent.x << ' '
					<< vertices[i].bitangent.y << ' '
					<< vertices[i].bitangent.z << '\n';
	}

	// add indices
	dataStream << '\n' << indices.Size() << '\n';
	// populate array
	for ( int i = 0; i < indices.Size(); i += 3 ) {
		dataStream << indices[i + 0] << ' '
					<< indices[i + 1] << ' '
					<< indices[i + 2] << '\n';
	}

	// write file
	return String( dataStream.str().c_str() );
}

