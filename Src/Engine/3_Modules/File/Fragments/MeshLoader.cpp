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

	// parse data and get values
	Char* dataPtr = NO_CONST( Char*, data );
	// jump over header
	dataPtr += strlen( HEADER );

	// read vertices
	UInt countVertices = strtol( dataPtr, &dataPtr, 10 );
	for ( UInt i = 0; i < countVertices; i++ ) {
		// read position
		Vector3 pos( strtof( dataPtr, &dataPtr ), 
					 strtof( dataPtr, &dataPtr ), 
					 strtof( dataPtr, &dataPtr ));
		// read normal
		Vector3 normal( strtof( dataPtr, &dataPtr ), 
						strtof( dataPtr, &dataPtr ), 
						strtof( dataPtr, &dataPtr ));
		// read uv
		Vector2 uv( strtof( dataPtr, &dataPtr ), 
					strtof( dataPtr, &dataPtr ));
		// read color
		Vector4 color( strtof( dataPtr, &dataPtr ), 
					   strtof( dataPtr, &dataPtr ), 
					   strtof( dataPtr, &dataPtr ), 
					   strtof( dataPtr, &dataPtr ));

		// create vertex
		vertices.Add( 
			Vertex(
				Vector3( pos.x, pos.y, pos.z ),
				Vector4( color.x, color.y, color.z, color.w ),
				Vector3( normal.x, normal.y, normal.z ),
				Vector2( uv.x, uv.y )
			)
		);
	}

	// read indeces
	UInt countIndices = strtol( dataPtr, &dataPtr, 10 );
	for ( UInt i = 0; i < countIndices; i++ ) {
		indices.Add( strtol( dataPtr, &dataPtr, 10 ) );
	}

	return TRUE;
}


String MeshLoader::SaveMesh( DataArray<Vertex>& vertices, DataArray<UInt>& indices )
{
	// create string
	std::ostringstream dataStream;

	// add header
	dataStream << HEADER;

	// add vertices
	dataStream << '\n' << vertices.Size() << '\n';
	// populate array
	for ( UInt i = 0; i < vertices.Size(); i++ ) {
		dataStream << vertices[i].position.x << ' '
					<< vertices[i].position.y << ' '
					<< vertices[i].position.z << ' '
					<< vertices[i].normal.x << ' '
					<< vertices[i].normal.y << ' '
					<< vertices[i].normal.z << ' '
					<< vertices[i].uv.x << ' '
					<< vertices[i].uv.y << ' '
					<< vertices[i].color.x << ' '
					<< vertices[i].color.y << ' '
					<< vertices[i].color.z << ' '
					<< vertices[i].color.w << '\n';
	}

	// add indices
	dataStream << '\n' << indices.Size() << '\n';
	// populate array
	for ( UInt i = 0; i < indices.Size() / 3; i += 3 ) {
		dataStream << indices[i + 0] << ' '
					<< indices[i + 1] << ' '
					<< indices[i + 2] << '\n';
	}

	// write file
	return String( dataStream.str().c_str() );
}

