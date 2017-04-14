-- add vertices
MESH:ClearVertices()

local rings = 33
local sectors = 36
local radius = 0.5

local R = 1.0 / (rings - 1)
local S = 1.0 / (sectors - 1)

for r = 0, rings - 1, 1 do
	for s = 0, sectors - 1, 1 do
		local y = math.sin( math.pi / 2 - math.pi * r * R )
		local x = math.cos( 2 * math.pi * s * S ) * math.sin( math.pi * r * R )
		local z = math.sin( 2 * math.pi * s * S ) * math.sin( math.pi * r * R )

		MESH:AddVertex(	Vertex(
			Vector3( x * radius, y * radius, z * radius ),	-- pos
			Vector4( 1, 0, 0, 1 ),							-- color
			Vector3( x, y, z ),								-- normal
			Vector2( s * S, r * R ),							-- uv
			Vector3( 0, 0, 0 ),								-- tan
			Vector3( 0, 0, 0 )								-- bitan
		))
	end
end



-- add indices
MESH:ClearIndices()

for r = 0, rings - 2, 1 do
	for s = 0, sectors - 2, 1 do

		MESH:AddIndex( r * sectors + s 			)
		MESH:AddIndex( r * sectors + (s+1) 		)
		MESH:AddIndex( (r+1) * sectors + (s+1) 	)
 
		MESH:AddIndex( r * sectors + s 			)
		MESH:AddIndex( (r+1) * sectors + (s+1) 	)
		MESH:AddIndex( (r+1) * sectors + s 		)

	end
end
