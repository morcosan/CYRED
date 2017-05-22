-- add vertices
MESH:ClearVertices()

local color = Vector4( 0.5, 0.5, 1, 1 )

MESH:AddVertex( Vertex( Vector3(-1, -1, -1), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
MESH:AddVertex( Vertex( Vector3( 1, -1, -1), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
MESH:AddVertex( Vertex( Vector3( 1,  1, -1), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
MESH:AddVertex( Vertex( Vector3(-1,  1, -1), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )

MESH:AddVertex( Vertex( Vector3(-1, -1, 0), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
MESH:AddVertex( Vertex( Vector3( 1, -1, 0), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
MESH:AddVertex( Vertex( Vector3( 1,  1, 0), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
MESH:AddVertex( Vertex( Vector3(-1,  1, 0), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )


-- add indices
MESH:ClearIndices()

MESH:AddIndex(0) MESH:AddIndex(1)
MESH:AddIndex(1) MESH:AddIndex(2)
MESH:AddIndex(2) MESH:AddIndex(3)
MESH:AddIndex(3) MESH:AddIndex(0)

MESH:AddIndex(4) MESH:AddIndex(5)
MESH:AddIndex(5) MESH:AddIndex(6)
MESH:AddIndex(6) MESH:AddIndex(7)
MESH:AddIndex(7) MESH:AddIndex(4)

MESH:AddIndex(0) MESH:AddIndex(4)
MESH:AddIndex(1) MESH:AddIndex(5)
MESH:AddIndex(2) MESH:AddIndex(6)
MESH:AddIndex(3) MESH:AddIndex(7)