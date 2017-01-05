VARS = {
	speed 		= "FLOAT",
	accOnBounce	= "FLOAT",
	limits		= "VECTOR2",
	lives 		= "INT"
}



local transform 
local direction
local currSpeed
local ballSize
local canBounce
local padLeft
local padRight
local livesLeft = {}
local livesRight = {}


function OnStart()
	transform = GAMEOBJECT:GetComponent_Transform()
	
	-- set ball size
	ballSize = transform.scaleLocal.x

	-- store pad transform
	padLeft = SCENE:Search( "PadLeft", nil ):GetComponent_Transform()
	padRight = SCENE:Search( "PadRight", nil ):GetComponent_Transform()

	-- store score points
	for i = 1, VARS.lives do
		table.insert( livesLeft, SCENE:Search( "LifeLeft " .. i, nil ) )
		table.insert( livesRight, SCENE:Search( "LifeRight " .. i, nil ) )
	end
	livesLeft.count = VARS.lives
	livesRight.count = VARS.lives

	-- start game
	Restart()
end


function OnUpdate()
	local velocity = Vector3(0, 0, 0)
	velocity.x = direction.x * currSpeed * TIME:GetDeltaTime()
	velocity.y = direction.y * currSpeed * TIME:GetDeltaTime()
	transform:TranslateByLocal( velocity )

	local ballPos = transform.positionWorld
	local padPosL = padLeft.positionWorld
	local padPosR = padRight.positionWorld

	-- bounce top
	if (ballPos.y + ballSize / 2) >= VARS.limits.y then
		-- change direction
		direction.y = - direction.y
		-- increase speed
		currSpeed = currSpeed + VARS.accOnBounce
	end

	-- bounce bottom
	if (ballPos.y - ballSize / 2) <= - VARS.limits.y then
		-- change direction
		direction.y = - direction.y
		-- increase speed
		currSpeed = currSpeed + VARS.accOnBounce
	end

	-- pass left
	if (ballPos.x - ballSize / 2) <= padPosL.x then
		canBounce = false
	end

	-- pass right
	if (ballPos.x + ballSize / 2) >= padPosR.x then
		canBounce = false
	end

	-- bounce left pad
	if canBounce and (ballPos.x - ballSize / 2) <= (padPosL.x + g_padSize.x / 2) then
		if (ballPos.y + ballSize / 2) >= (padPosL.y - g_padSize.y / 2) and 
			(ballPos.y - ballSize / 2) <= (padPosL.y + g_padSize.y / 2)
		then
			-- change direction
			direction.x = - direction.x
			-- increase speed
			currSpeed = currSpeed + VARS.accOnBounce
		else
			-- pass left
			canBounce = false
		end
	end

	-- bounce right pad
	if canBounce and (ballPos.x + ballSize / 2) >= (padPosR.x - g_padSize.x / 2) then
		if (ballPos.y + ballSize / 2) >= (padPosR.y - g_padSize.y / 2) and 
			(ballPos.y - ballSize / 2) <= (padPosR.y + g_padSize.y / 2)
		then
			-- change direction
			direction.x = - direction.x
			-- increase speed
			currSpeed = currSpeed + VARS.accOnBounce
		else
			-- pass left
			canBounce = false
		end
	end

	-- exit left
	if ballPos.x <= - VARS.limits.x then
		-- score for right
		livesLeft[ livesLeft.count ].enabled = false
		livesLeft.count = livesLeft.count - 1

		if livesLeft.count == 0 then
			-- quit game
			APP:Exit()
		else
			-- next round
			Restart()
		end
	end

	-- exit right
	if ballPos.x >= VARS.limits.x then
		-- score for left
		livesRight[ livesRight.count ].enabled = false
		livesRight.count = livesRight.count - 1

		if livesRight.count == 0 then
			-- quit game
			APP:Exit()
		else
			-- next round
			Restart()
		end
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