-- add vertices
MESH:ClearVertices()

MESH:AddVertex( Vertex( Vector3(-1, -1, -1), Vector4(0,0,0,0), Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
MESH:AddVertex( Vertex( Vector3( 1, -1, -1), Vector4(0,0,0,0), Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
MESH:AddVertex( Vertex( Vector3( 1,  1, -1), Vector4(0,0,0,0), Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
MESH:AddVertex( Vertex( Vector3(-1,  1, -1), Vector4(0,0,0,0), Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )

MESH:AddVertex( Vertex( Vector3(0, 0, 0), Vector4(0,0,0,0), Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )


-- add indices
MESH:ClearIndices()

MESH:AddIndex(0) MESH:AddIndex(1)
MESH:AddIndex(1) MESH:AddIndex(2)
MESH:AddIndex(2) MESH:AddIndex(3)
MESH:AddIndex(3) MESH:AddIndex(0)

MESH:AddIndex(0) MESH:AddIndex(4)
MESH:AddIndex(1) MESH:AddIndex(4)
MESH:AddIndex(2) MESH:AddIndex(4)
MESH:AddIndex(3) MESH:AddIndex(4)
