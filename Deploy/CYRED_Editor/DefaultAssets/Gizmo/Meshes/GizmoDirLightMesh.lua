-- add vertices
MESH:ClearVertices()

-- add indices
MESH:ClearIndices()


local radius 	= 0.1
local length 	= 1
local vertices 	= 12
local angle 	= 360 / vertices
local offet		= -(length - 2 * radius)
local color 	= Vector4( 1, 1, 0.5, 1 )

-- arrow pointer circle
for i = 0, vertices - 1, 1 do
	local x = radius * math.cos( i * angle )
	local y = radius * math.sin( i * angle )
	-- add vertex
	MESH:AddVertex( Vertex( Vector3(x, y, offet), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
	-- add indices
	MESH:AddIndex( i )
	MESH:AddIndex( (i + 1) % vertices )
end

-- arrow pointer height
MESH:AddVertex( Vertex( Vector3(0, 0, -length), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
for i = 0, vertices - 1, 1 do
	-- add indices
	MESH:AddIndex( vertices )
	MESH:AddIndex( i )
end

-- arrow line
MESH:AddVertex( Vertex( Vector3(0, 0, 0), 	  color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
MESH:AddVertex( Vertex( Vector3(0, 0, offet), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
-- add indices
MESH:AddIndex( vertices + 1 )
MESH:AddIndex( vertices + 2 )
