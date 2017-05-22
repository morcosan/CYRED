-- add vertices
MESH:ClearVertices()

-- add indices
MESH:ClearIndices()


local range 		= 1
local baseCount 	= 36
local heightCount 	= 4
local angle 		= 360 / baseCount

-- create base
for i = 0, baseCount - 1, 1 do
	local x = range * math.cos( i * angle )
	local y = range * math.sin( i * angle )
	-- add vertex
	MESH:AddVertex( Vertex( Vector3(x, y, -range), Vector4(0,0,0,0), Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
	-- add indices
	MESH:AddIndex( i )
	MESH:AddIndex( (i + 1) % baseCount )
end

-- create origin
MESH:AddVertex( Vertex( Vector3(0, 0, 0), Vector4(0,0,0,0), Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
for i = 0, baseCount - 1, (baseCount / heightCount) do
	-- add indices
	MESH:AddIndex( baseCount )
	MESH:AddIndex( i )
end
