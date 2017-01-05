VARS = {
	isLeft = "BOOL",
	speed = "FLOAT"
}

-- global
g_padSize = Vector2( 0, 0 )


local transform 


function OnStart()
	transform = GAMEOBJECT:GetComponent_Transform()

	-- set globals
	g_padSize.x = transform.scaleLocal.x
	g_padSize.y = transform.scaleLocal.y
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