-- add vertices
MESH:ClearVertices()

-- huge number
local huge = 2147483647
-- small offset to be above grid
local offset = 0.0001

-- colors
local colorX = Vector4( 1, 0.3, 0.3, 1 )
local colorY = Vector4( 0.3, 1, 0.3, 1 )
local colorZ = Vector4( 0.3, 0.3, 1, 1 )

-- X axis
MESH:AddVertex( Vertex( Vector3(0,    offset, 0), colorX, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) )             
MESH:AddVertex( Vertex( Vector3(huge, offset, 0), colorX, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) ) 

-- Y axis
MESH:AddVertex( Vertex( Vector3(0, 0, 0), 	 colorY, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) ) 
MESH:AddVertex( Vertex( Vector3(0, huge, 0), colorY, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) )

-- Z axis
MESH:AddVertex( Vertex( Vector3(0, offset, 0),    colorZ, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) ) 
MESH:AddVertex( Vertex( Vector3(0, offset, huge), colorZ, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0) ) )


-- add indices
MESH:ClearIndices()

MESH:AddIndex(0) MESH:AddIndex(1) 
MESH:AddIndex(2) MESH:AddIndex(3) 
MESH:AddIndex(4) MESH:AddIndex(5)
