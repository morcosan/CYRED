// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "MeshLoader.h"
#include "../FileManager.h"
#include "../../../2_BuildingBlocks/String/FiniteString.h"

#include <string>
#include <sstream>


using namespace CYRED;


Bool MeshLoader::LoadMesh( const Char* data, OUT DataArray<Vertex>& vertices, 
						   OUT DataArray<UInt>& indices )
{
	int headerIndex = strncmp( data, HEADER, strlen( HEADER ) );
	if ( headerIndex != 0 ) {
		return FALSE;
	}

	Char* dataPtr = NO_CONST( Char*, data );

	// read vertices
	UInt countVertices;
	countVertices = strtol( dataPtr, &dataPtr, 10 );
	for ( UInt i = 0; i < countVertices; i++ ) {
	}

	// read indeces
	int countIndeces;
	sscanf( data, VERTICES_FORMAT, &countIndeces );



	return TRUE;
}


String MeshLoader::SaveMesh( DataArray<Vertex>& vertices, DataArray<UInt>& indices )
{
	// create string
	std::ostringstream dataStream;
	// use custom format
	FiniteString format;

	// add header
	dataStream << HEADER;

	// add vertices
	format.Set( VERTICES_FORMAT, vertices.Size() );
	dataStream << format.GetChar();
	// populate array
	for ( UInt i = 0; i < vertices.Size(); i++ ) {
		format.Set( VERTEX_FORMAT, 
					vertices[i].position.x,
					vertices[i].position.y,
					vertices[i].position.z,
					vertices[i].normal.x,
					vertices[i].normal.y,
					vertices[i].normal.z,
					vertices[i].uv.x,
					vertices[i].uv.y,
					vertices[i].color.x,
					vertices[i].color.y,
					vertices[i].color.z,
					vertices[i].color.w
		);
		// write to file
		dataStream << format.GetChar();
	}

	// add indices
	format.Set( INDICES_FORMAT, indices.Size() / 3 );
	dataStream << format.GetChar();
	// populate array
	for ( UInt i = 0; i < indices.Size() / 3; i += 3 ) {
		format.Set( INDEX_FORMAT, 
					indices[i + 0],
					indices[i + 1],
					indices[i + 2]
		);
		// write to file
		dataStream << format.GetChar();
	}

	// write file
	return String( dataStream.str().c_str() );
}

