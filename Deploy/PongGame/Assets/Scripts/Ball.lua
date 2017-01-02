VARS = {
	speed 		= "FLOAT",
	accOnBounce	= "FLOAT",
	limits		= "VECTOR2" 
}



local transform 
local direction
local currSpeed
local ballSize

function OnStart()
	transform = GAMEOBJECT:GetComponent_Transform()
	-- set direction start
	direction = Vector3( -1, 0, 0 )
	-- set speed at start
	currSpeed = VARS.speed
	-- set ball size
	ballSize = transform.scaleLocal.x
end


function OnUpdate()
	local velocity = Vector3(0, 0, 0)
	velocity.x = direction.x * currSpeed * TIME:GetDeltaTime()
	velocity.y = direction.y * currSpeed * TIME:GetDeltaTime()
	transform:TranslateByLocal( velocity )

	-- bounce left
	if (transform.positionWorld - ballSize / 2) < - VARS.limits.x then

	end
end