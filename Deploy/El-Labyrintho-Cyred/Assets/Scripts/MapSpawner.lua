VARS = {
	wall 		= "PREFAB",
	floor 		= "PREFAB",
	startFloor 	= "PREFAB",
	endFloor 	= "PREFAB"
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

	-- get map holder
	local map = SCENE:FindGameObject( "Map" )
	-- clear prev map
	

	-- create level
	for ln = 1, height do
		for col = 1, width do
			local c = level_1[ln]:sub( col, col )
			local gameObject = nil

			if c == 'w' then
				gameObject = SCENE:Instantiate( VARS.wall, 0 )
			elseif c == '.' then
				gameObject = SCENE:Instantiate( VARS.floor, 0 )
			elseif c == 's' then
				gameObject = SCENE:Instantiate( VARS.startFloor, 0 )
			elseif c == 'e' then
				gameObject = SCENE:Instantiate( VARS.endFloor, 0 )
			end

			if gameObject ~= nil then
				-- add to map
				gameObject.parentNode = map
				-- position object
				local transform = gameObject:AddComponent_Transform()
				transform.positionWorld = Vector3( col, transform.positionWorld.y, ln )
			end
		end
	end
end