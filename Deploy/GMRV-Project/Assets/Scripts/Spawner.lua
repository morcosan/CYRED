VARS = {
	obstacle 		= "PREFAB",
	spawnInterval	= "FLOAT"
}


local timer = -1
local transform = nil


function OnStart()
	-- start spawning
	timer = VARS.spawnInterval

	transform = GAMEOBJECT:GetComponent_Transform()
end


function OnUpdate()
	if timer > 0 then
		timer = timer - TIME:GetDeltaTime()

		-- ready to spawn
		if timer < 0 then
			-- create obstacle
			local obstacle = SCENE:Instantiate( VARS.obstacle, 0 )
			local obstacleTran = obstacle:GetComponent_Transform()
			local x = Random.FromRangeFloat( -transform.scaleWorld.x, transform.scaleWorld.x )
			local z = Random.FromRangeFloat( -transform.scaleWorld.z, transform.scaleWorld.z )
			obstacleTran.positionWorld = Vector3( x / 2, transform.positionWorld.y, z / 2 )
			obstacle.parentNode = GAMEOBJECT
			
			-- reset
			timer = VARS.spawnInterval
		end
	end
end