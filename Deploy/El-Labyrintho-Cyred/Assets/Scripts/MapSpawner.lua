VARS = {
	wall = "PREFAB",
	floor = "PREFAB"
}


local level_1 = {
	"wwwwwwwwwww--",
	"w.....w...w--",
	"w.www...w.w--",
	"w.w.w.www.e--",
	"w.w..s....w--",
	"wwwwwwwwwww--"
}


function OnStart()
	local height 	= #level_1
	local width 	= #level_1[1]

	-- create level
	for ln = 1, height do
		for col = 1, width do
			local c = level_1[ln]:sub( col, col )
			local gameObject = nil

			if c == 'w' then
				gameObject = SCENE:Instantiate( VARS.wall, 0 )
			elseif c == '.' then
				gameObject = SCENE:Instantiate( VARS.floor, 0 )
			end

			if gameObject ~= nil then
				local transform = gameObject:GetComponent_Transform()
				transform.positionWorld = Vector3( ln, transform.positionWorld.y, col )
			end
		end
	end
end