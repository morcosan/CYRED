VARS = {
	speed 		= "FLOAT",
	accOnBounce	= "FLOAT",
	limits		= "VECTOR2" 
}



local transform 
local direction
local currSpeed
local ballSize
local canBounce


function OnStart()
	transform = GAMEOBJECT:GetComponent_Transform()
	
	-- set ball size
	ballSize = transform.scaleLocal.x

	-- start game
	Restart()
end


function OnUpdate()
	local velocity = Vector3(0, 0, 0)
	velocity.x = direction.x * currSpeed * TIME:GetDeltaTime()
	velocity.y = direction.y * currSpeed * TIME:GetDeltaTime()
	transform:TranslateByLocal( velocity )

	-- bounce top
	if (transform.positionWorld.y + ballSize / 2) >= VARS.limits.y then
		-- change direction
		direction.y = - direction.y
	end

	-- bounce bottom
	if (transform.positionWorld.y - ballSize / 2) <= - VARS.limits.y then
		-- change direction
		direction.y = - direction.y
	end

	-- pass left
	if (transform.positionWorld.x - ballSize / 2) <= g_padLeftX then
		canBounce = false
	end

	-- pass right
	if (transform.positionWorld.x - ballSize / 2) >= g_padRightX then
		canBounce = false
	end

	-- bounce left pad
	if (transform.positionWorld.x - ballSize / 2) <= - VARS.limits.y then
		-- change direction
		direction.y = - direction.y
	end

	-- exit left
	if (transform.positionWorld.x - ballSize / 2) <= - VARS.limits.x then
		-- score for right
	end

	-- exit right
	if (transform.positionWorld.x + ballSize / 2) >= VARS.limits.x then
		-- score for left
	end
end



function Restart()
	-- can bounce
	canBounce = true
	-- reset position
	transform.positionWorld = Vector3( 0, 0, 0 )
	-- set direction start
	local dir = Random.FromCircleEdge( 1 )
	direction = Vector3( dir.x, dir.y, 0 )
	-- set speed at start
	currSpeed = VARS.speed
end