-- add vertices
MESH:ClearVertices()

-- add indices
MESH:ClearIndices()


local range 		= 1
local radius 		= 0.5
local baseCount 	= 36
local heightCount 	= 12
local angle 		= 2 * math.pi / baseCount
local color 		= Vector4( 1, 1, 0.5, 1 )

-- create base
for i = 0, baseCount - 1, 1 do
	local x = radius * math.cos( i * angle )
	local y = radius * math.sin( i * angle )
	-- add vertex
	MESH:AddVertex( Vertex( Vector3(x, y, -range), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
	-- add indices
	MESH:AddIndex( i )
	MESH:AddIndex( (i + 1) % baseCount )
end

-- create origin
MESH:AddVertex( Vertex( Vector3(0, 0, 0), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
for i = 0, baseCount - 1, (baseCount / heightCount) do
	-- add indices
	MESH:AddIndex( baseCount )
	MESH:AddIndex( i )
end
