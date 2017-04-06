-- add vertices
MESH:StartVertices()

MESH:AddVertex( Vertex(
	Vector3( 0, 0, 0 ), 
	Vector4( 0, 0, 0, 0 ), 
	Vector3( 0, 0, 0 ), 
	Vector2( 0, 0 ), 
	Vector3( 0, 0, 0 ), 
	Vector3( 0, 0, 0 )
) )

MESH:FinishVertices()



-- add indices
MESH:StartIndices()

MESH:AddIndex( 0 )

MESH:FinishIndices()