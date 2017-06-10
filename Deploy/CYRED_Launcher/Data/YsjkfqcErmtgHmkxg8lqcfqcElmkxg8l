-- add vertices
MESH:ClearVertices()

-- huge number
local size = 1

-- colors
local colorX = Vector4( 1, 0.3, 0.3, 1 )
local colorY = Vector4( 0.3, 1, 0.3, 1 )
local colorZ = Vector4( 0.3, 0.3, 1, 1 )

-- X axis
MESH:AddVertex( Vertex( Vector3(0,    0, 0), colorX, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) )             
MESH:AddVertex( Vertex( Vector3(size, 0, 0), colorX, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) ) 

-- Y axis
MESH:AddVertex( Vertex( Vector3(0, 0, 0), 	 colorY, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) ) 
MESH:AddVertex( Vertex( Vector3(0, size, 0), colorY, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) )

-- Z axis
MESH:AddVertex( Vertex( Vector3(0, 0, 0),    colorZ, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) ) 
MESH:AddVertex( Vertex( Vector3(0, 0, size), colorZ, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) )


-- add indices
MESH:ClearIndices()

MESH:AddIndex(0) MESH:AddIndex(1) 
MESH:AddIndex(2) MESH:AddIndex(3) 
MESH:AddIndex(4) MESH:AddIndex(5)
