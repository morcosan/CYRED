// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "MeshLoader.h"

#include "Assimp\Include\scene.h"
#include "Assimp\Include\Importer.hpp"
#include "Assimp\Include\postprocess.h"


using namespace CYRED;


Bool MeshLoader::LoadSingleMesh( const Char* buffer, UInt bufferSize, 
								 DataArray<Vertex>& vertices, DataArray<UInt>& indices )
{
	Assimp::Importer assimpImporter;

	const aiScene* assimpScene = assimpImporter.ReadFileFromMemory( buffer, bufferSize, aiProcess_Triangulate );
	if ( assimpScene == NULL )
	{
		return FALSE;
	}

	for ( UInt i = 0; i < assimpScene->mNumMeshes; ++i )
	{
		aiMesh* assimpMesh = assimpScene->mMeshes[i];

		Bool needTanBitan = FALSE;

		aiVector3D zero3D(0, 0, 0);
		aiColor4D zero4D(0, 0, 0, 0);

		for ( UInt j = 0; j < assimpMesh->mNumVertices; ++j ) 
		{
			aiVector3D* assimpPos = assimpMesh->HasPositions() ? &(assimpMesh->mVertices[j]) : &zero3D;
			aiColor4D* assimpColor = assimpMesh->HasVertexColors(0) ? &(assimpMesh->mColors[0][j]) : &zero4D;
			aiVector3D* assimpNormal = assimpMesh->HasNormals() ? &(assimpMesh->mNormals[j]) : &zero3D;
			aiVector3D* assimpUV = assimpMesh->HasTextureCoords(0) ? &(assimpMesh->mTextureCoords[0][j]) : &zero3D;
			aiVector3D* assimpTan = assimpMesh->HasTangentsAndBitangents() ? &(assimpMesh->mTangents[j]) : &zero3D;
			aiVector3D* assimpBitan = assimpMesh->HasTangentsAndBitangents() ? &(assimpMesh->mBitangents[j]) : &zero3D;
			
			if ( assimpTan->Length() < 0.001f || assimpBitan->Length() < 0.001f )
			{
				needTanBitan = true;
			}

			vertices.Add( 
				Vertex(
					Vector3( assimpPos->x, assimpPos->y, assimpPos->z ),
					Vector4( assimpColor->r, assimpColor->g, assimpColor->b, assimpColor->a ),
					Vector3( assimpNormal->x, assimpNormal->y, assimpNormal->z ),
					Vector2( assimpUV->x, assimpUV->y ),
					Vector3( assimpTan->x, assimpTan->y, assimpTan->z ),
					Vector3( assimpBitan->x, assimpBitan->y, assimpBitan->z )
				)
			);
		}

		if ( assimpMesh->HasFaces() )
		{
			for ( UInt j = 0; j < assimpMesh->mNumFaces; ++j ) 
			{
				aiFace& assimpFace = assimpMesh->mFaces[j];
				indices.Add( assimpFace.mIndices[0] );
				indices.Add( assimpFace.mIndices[1] );
				indices.Add( assimpFace.mIndices[2] );
			}
		}
	}

	return TRUE;
}
