-- add vertices
MESH:ClearVertices()

local size  = 10
local space = 0.5
local color = Vector4( 0.5, 0.5, 0.5, 1 )

for x = - size / 2, size / 2, 1 do
	MESH:AddVertex( Vertex( Vector3(x * space, 0, -size/2 * space),	color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) );
	MESH:AddVertex( Vertex( Vector3(x * space, 0,  size/2 * space),	color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) );
end
for z = - size / 2, size / 2, 1 do
	MESH:AddVertex( Vertex( Vector3(-size/2 * space, 0, z * space),	color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) );
	MESH:AddVertex( Vertex( Vector3( size/2 * space, 0, z * space),	color, Vector3(0,0,0), Vector2(0, 0), Vector3(0,0,0), Vector3(0,0,0)) );
end


-- add indices
MESH:ClearIndices()

for i = 0, 2 * 2 * (size + 1) - 1, 1 do
	MESH:AddIndex(i)
end
