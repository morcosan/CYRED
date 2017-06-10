-- add vertices
MESH:ClearVertices()

-- add indices
MESH:ClearIndices()


local radius 	= 1
local vertices 	= 36
local angle 	= 2 * math.pi / vertices
local color 	= Vector4( 1, 1, 0.5, 1 )
local lastIndex = 0
local angleSin  = math.pi / 12
local sinOffset	= math.sin( angleSin )
local sinFactor	= math.sin( math.pi / 2 - angleSin )


-- create x-y circle x3

for i = 0, vertices - 1, 1 do
	local x = radius * math.cos( i * angle )
	local y = radius * math.sin( i * angle )
	local z = 0
	-- add vertecies
	MESH:AddVertex( Vertex( Vector3(x, y, z), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
	-- add indices
	MESH:AddIndex( lastIndex + i )
	MESH:AddIndex( lastIndex + (i + 1) % vertices )
end
lastIndex = lastIndex + vertices

for i = 0, vertices - 1, 1 do
	local x = radius * math.cos( i * angle ) * sinFactor
	local y = radius * math.sin( i * angle ) * sinFactor
	local z = radius * sinOffset
	-- add vertecies
	MESH:AddVertex( Vertex( Vector3(x, y, z), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
	-- add indices
	MESH:AddIndex( lastIndex + i )
	MESH:AddIndex( lastIndex + (i + 1) % vertices )
end
lastIndex = lastIndex + vertices

for i = 0, vertices - 1, 1 do
	local x = radius * math.cos( i * angle ) * sinFactor
	local y = radius * math.sin( i * angle ) * sinFactor
	local z = - radius * sinOffset
	-- add vertecies
	MESH:AddVertex( Vertex( Vector3(x, y, z), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
	-- add indices
	MESH:AddIndex( lastIndex + i )
	MESH:AddIndex( lastIndex + (i + 1) % vertices )
end
lastIndex = lastIndex + vertices



-- create x-z circle x3

for i = 0, vertices - 1, 1 do
	local x = radius * math.cos( i * angle )
	local y = 0
	local z = radius * math.sin( i * angle )
	-- add vertex
	MESH:AddVertex( Vertex( Vector3(x, y, z), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
	-- add indices
	MESH:AddIndex( lastIndex + i )
	MESH:AddIndex( lastIndex + (i + 1) % vertices )
end
lastIndex = lastIndex + vertices

for i = 0, vertices - 1, 1 do
	local x = radius * math.cos( i * angle ) * sinFactor
	local y = radius * sinOffset
	local z = radius * math.sin( i * angle ) * sinFactor
	-- add vertex
	MESH:AddVertex( Vertex( Vector3(x, y, z), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
	-- add indices
	MESH:AddIndex( lastIndex + i )
	MESH:AddIndex( lastIndex + (i + 1) % vertices )
end
lastIndex = lastIndex + vertices

for i = 0, vertices - 1, 1 do
	local x = radius * math.cos( i * angle ) * sinFactor
	local y = - radius * sinOffset
	local z = radius * math.sin( i * angle ) * sinFactor
	-- add vertex
	MESH:AddVertex( Vertex( Vector3(x, y, z), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
	-- add indices
	MESH:AddIndex( lastIndex + i )
	MESH:AddIndex( lastIndex + (i + 1) % vertices )
end
lastIndex = lastIndex + vertices



-- create y-z circle x3

for i = 0, vertices - 1, 1 do
	local x = 0
	local y = radius * math.cos( i * angle )
	local z = radius * math.sin( i * angle )
	-- add vertex
	MESH:AddVertex( Vertex( Vector3(x, y, z), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
	-- add indices
	MESH:AddIndex( lastIndex + i )
	MESH:AddIndex( lastIndex + (i + 1) % vertices )
end
lastIndex = lastIndex + vertices

for i = 0, vertices - 1, 1 do
	local x = radius * sinOffset
	local y = radius * math.cos( i * angle ) * sinFactor
	local z = radius * math.sin( i * angle ) * sinFactor
	-- add vertex
	MESH:AddVertex( Vertex( Vector3(x, y, z), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
	-- add indices
	MESH:AddIndex( lastIndex + i )
	MESH:AddIndex( lastIndex + (i + 1) % vertices )
end
lastIndex = lastIndex + vertices

for i = 0, vertices - 1, 1 do
	local x = - radius * sinOffset
	local y = radius * math.cos( i * angle ) * sinFactor
	local z = radius * math.sin( i * angle ) * sinFactor
	-- add vertex
	MESH:AddVertex( Vertex( Vector3(x, y, z), color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) )
	-- add indices
	MESH:AddIndex( lastIndex + i )
	MESH:AddIndex( lastIndex + (i + 1) % vertices )
end
lastIndex = lastIndex + vertices


