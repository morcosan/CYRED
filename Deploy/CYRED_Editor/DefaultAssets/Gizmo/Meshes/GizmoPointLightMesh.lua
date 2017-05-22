-- add vertices
MESH:ClearVertices()

-- add indices
MESH:ClearIndices()


local radius 	= 1
local vertices 	= 36
local angle 	= 2 * math.pi / vertices

local lastIndex 	= 0

-- create x-y circle
for i = 0, vertices - 1, 1 do
	local x = radius * math.cos( i * angle )
	local y = radius * math.sin( i * angle )
	-- add vertex
	MESH:AddVertex( Vertex( Vector3(x, y, 0), Vector4(0,0,0,0), Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
	-- add indices
	MESH:AddIndex( lastIndex + i )
	MESH:AddIndex( lastIndex + (i + 1) % vertices )
end


lastIndex = vertices

-- create x-z circle
for i = 0, vertices - 1, 1 do
	local x = radius * math.cos( i * angle )
	local z = radius * math.sin( i * angle )
	-- add vertex
	MESH:AddVertex( Vertex( Vector3(x, 0, z), Vector4(0,0,0,0), Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
	-- add indices
	MESH:AddIndex( lastIndex + i )
	MESH:AddIndex( lastIndex + (i + 1) % vertices )
end


lastIndex = 2 * vertices

-- create y-z circle
for i = 0, vertices - 1, 1 do
	local y = radius * math.cos( i * angle )
	local z = radius * math.sin( i * angle )
	-- add vertex
	MESH:AddVertex( Vertex( Vector3(0, y, z), Vector4(0,0,0,0), Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
	-- add indices
	MESH:AddIndex( lastIndex + i )
	MESH:AddIndex( lastIndex + (i + 1) % vertices )
end


