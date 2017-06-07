-- add vertices
MESH:ClearVertices()

local color 	= Vector4( 0.5, 0.5, 1, 1 )
local normal 	= Vector3( 0, 0, 0 )
local uv 		= Vector2( 0, 0 )
local tan 		= Vector3( 0, 0, 0 )
local bitan 	= Vector3( 0, 0, 0 )

MESH:AddVertex( Vertex( Vector3(-0.5, -0.5, -1), color, normal, uv, tan, bitan ) )
MESH:AddVertex( Vertex( Vector3( 0.5, -0.5, -1), color, normal, uv, tan, bitan ) )
MESH:AddVertex( Vertex( Vector3( 0.5,  0.5, -1), color, normal, uv, tan, bitan ) )
MESH:AddVertex( Vertex( Vector3(-0.5,  0.5, -1), color, normal, uv, tan, bitan ) )

MESH:AddVertex( Vertex( Vector3(-0.5, -0.5, 0), color, normal, uv, tan, bitan ) )
MESH:AddVertex( Vertex( Vector3( 0.5, -0.5, 0), color, normal, uv, tan, bitan ) )
MESH:AddVertex( Vertex( Vector3( 0.5,  0.5, 0), color, normal, uv, tan, bitan ) )
MESH:AddVertex( Vertex( Vector3(-0.5,  0.5, 0), color, normal, uv, tan, bitan ) )


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
