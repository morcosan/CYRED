VARS = {
	isLeft = "BOOL",
	speed = "FLOAT"
}

-- global
g_padSize = {
	x = 0,
	y = 0
}
g_padLeftX = 0
g_padRightX = 0

local transform 


function OnStart()
	transform = GAMEOBJECT:GetComponent_Transform()

	-- set globals
	if VARS.isLeft then
		g_padLeftX = transform.positionWorld.x
	else
		g_padRightX = transform.positionWorld.x
	end
end


function OnUpdate()
	if VARS.isLeft then
		if INPUT:KeyDown( KEY_S ) then
			transform:TranslateByLocal( Vector3( 0, - VARS.speed * TIME:GetDeltaTime(), 0 ) )
		end
		if INPUT:KeyDown( KEY_W ) then
			transform:TranslateByLocal( Vector3( 0, VARS.speed * TIME:GetDeltaTime(), 0 ) )
		end
	else
		if INPUT:KeyDown( KEY_ARROW_DOWN ) then
			transform:TranslateByLocal( Vector3( 0, - VARS.speed * TIME:GetDeltaTime(), 0 ) )
		end
		if INPUT:KeyDown( KEY_ARROW_UP ) then
			transform:TranslateByLocal( Vector3( 0, VARS.speed * TIME:GetDeltaTime(), 0 ) )
		end
	end
end