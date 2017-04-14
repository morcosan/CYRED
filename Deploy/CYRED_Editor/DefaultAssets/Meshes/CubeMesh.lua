-- add vertices
MESH:ClearVertices()

-- front
MESH:AddVertex( Vertex( Vector3(-0.5,-0.5, 0.5), Vector4(1,0,0,1), Vector3(0,0,1), Vector2(0, 1), Vector3(0,0,0), Vector3(0,0,0) ) )             
MESH:AddVertex( Vertex( Vector3( 0.5,-0.5, 0.5), Vector4(0,1,0,1), Vector3(0,0,1), Vector2(1, 1), Vector3(0,0,0), Vector3(0,0,0) ) ) 
MESH:AddVertex( Vertex( Vector3( 0.5, 0.5, 0.5), Vector4(0,0,1,1), Vector3(0,0,1), Vector2(1, 0), Vector3(0,0,0), Vector3(0,0,0) ) )
MESH:AddVertex( Vertex( Vector3(-0.5, 0.5, 0.5), Vector4(1,0,0,1), Vector3(0,0,1), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) )

-- back
MESH:AddVertex( Vertex( Vector3(-0.5, -0.5, -0.5), Vector4(0,1,0,1), Vector3(0,0,-1), Vector2(1, 1), Vector3(0,0,0), Vector3(0,0,0) ) )             
MESH:AddVertex( Vertex( Vector3(-0.5,  0.5, -0.5), Vector4(0,0,1,1), Vector3(0,0,-1), Vector2(1, 0), Vector3(0,0,0), Vector3(0,0,0) ) ) 
MESH:AddVertex( Vertex( Vector3( 0.5,  0.5, -0.5), Vector4(1,0,0,1), Vector3(0,0,-1), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) ) 
MESH:AddVertex( Vertex( Vector3( 0.5, -0.5, -0.5), Vector4(0,1,0,1), Vector3(0,0,-1), Vector2(0, 1), Vector3(0,0,0), Vector3(0,0,0) ) )

-- top
MESH:AddVertex( Vertex( Vector3(-0.5, 0.5, -0.5), Vector4(0,0,1,1), Vector3(0,1,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) )             
MESH:AddVertex( Vertex( Vector3(-0.5, 0.5,  0.5), Vector4(1,0,0,1), Vector3(0,1,0), Vector2(0, 1), Vector3(0,0,0), Vector3(0,0,0) ) ) 
MESH:AddVertex( Vertex( Vector3( 0.5, 0.5,  0.5), Vector4(0,1,0,1), Vector3(0,1,0), Vector2(1, 1), Vector3(0,0,0), Vector3(0,0,0) ) ) 
MESH:AddVertex( Vertex( Vector3( 0.5, 0.5, -0.5), Vector4(0,0,1,1), Vector3(0,1,0), Vector2(1, 0), Vector3(0,0,0), Vector3(0,0,0) ) )

-- bottom
MESH:AddVertex( Vertex( Vector3(-0.5, -0.5, -0.5), Vector4(1,0,0,1), Vector3(0,-1,0), Vector2(0, 1), Vector3(0,0,0), Vector3(0,0,0) ) )             
MESH:AddVertex( Vertex( Vector3( 0.5, -0.5, -0.5), Vector4(0,1,0,1), Vector3(0,-1,0), Vector2(1, 1), Vector3(0,0,0), Vector3(0,0,0) ) ) 
MESH:AddVertex( Vertex( Vector3( 0.5, -0.5,  0.5), Vector4(0,0,1,1), Vector3(0,-1,0), Vector2(1, 0), Vector3(0,0,0), Vector3(0,0,0) ) ) 
MESH:AddVertex( Vertex( Vector3(-0.5, -0.5,  0.5), Vector4(1,0,0,1), Vector3(0,-1,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) )

-- right
MESH:AddVertex( Vertex( Vector3(0.5, -0.5, -0.5), Vector4(0,1,0,1), Vector3(1,0,0), Vector2(1, 1), Vector3(0,0,0), Vector3(0,0,0) ) )             
MESH:AddVertex( Vertex( Vector3(0.5,  0.5, -0.5), Vector4(0,0,1,1), Vector3(1,0,0), Vector2(1, 0), Vector3(0,0,0), Vector3(0,0,0) ) ) 
MESH:AddVertex( Vertex( Vector3(0.5,  0.5,  0.5), Vector4(1,0,0,1), Vector3(1,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) ) 
MESH:AddVertex( Vertex( Vector3(0.5, -0.5,  0.5), Vector4(0,1,0,1), Vector3(1,0,0), Vector2(0, 1), Vector3(0,0,0), Vector3(0,0,0) ) )

-- left
MESH:AddVertex( Vertex( Vector3(-0.5, -0.5, -0.5), Vector4(0,0,1,1), Vector3(-1,0,0), Vector2(0, 1), Vector3(0,0,0), Vector3(0,0,0) ) )             
MESH:AddVertex( Vertex( Vector3(-0.5, -0.5,  0.5), Vector4(1,0,0,1), Vector3(-1,0,0), Vector2(1, 1), Vector3(0,0,0), Vector3(0,0,0) ) ) 
MESH:AddVertex( Vertex( Vector3(-0.5,  0.5,  0.5), Vector4(0,1,0,1), Vector3(-1,0,0), Vector2(1, 0), Vector3(0,0,0), Vector3(0,0,0) ) ) 
MESH:AddVertex( Vertex( Vector3(-0.5,  0.5, -0.5), Vector4(0,0,1,1), Vector3(-1,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) )



-- add indices
MESH:ClearIndices()

-- front
MESH:AddIndex(0) MESH:AddIndex(1) MESH:AddIndex(2)
MESH:AddIndex(0) MESH:AddIndex(2) MESH:AddIndex(3)
-- back
MESH:AddIndex(4) MESH:AddIndex(5) MESH:AddIndex(6)
MESH:AddIndex(4) MESH:AddIndex(6) MESH:AddIndex(7)
-- top
MESH:AddIndex(8) MESH:AddIndex(9) MESH:AddIndex(10)
MESH:AddIndex(8) MESH:AddIndex(10) MESH:AddIndex(11)
-- bottom
MESH:AddIndex(12) MESH:AddIndex(13) MESH:AddIndex(14)
MESH:AddIndex(12) MESH:AddIndex(14) MESH:AddIndex(15)
-- right
MESH:AddIndex(16) MESH:AddIndex(17) MESH:AddIndex(18)
MESH:AddIndex(16) MESH:AddIndex(18) MESH:AddIndex(19)
-- left
MESH:AddIndex(20) MESH:AddIndex(21) MESH:AddIndex(22)
MESH:AddIndex(20) MESH:AddIndex(22) MESH:AddIndex(23)

