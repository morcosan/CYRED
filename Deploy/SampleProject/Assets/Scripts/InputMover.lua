VARS = {
	speed = "FLOAT"
}


local transform


function OnStart()
	transform = GAMEOBJECT:GetComponent_Transform()
end


function OnUpdate()
	if INPUT:KeyDown( KEY_ARROW_UP ) then
		transform:TranslateByWorld( Vector3( 0, VARS.speed * TIME:GetDeltaTime(), 0 ) );
	end

	if INPUT:KeyDown( KEY_ARROW_DOWN ) then
		transform:TranslateByWorld( Vector3( 0, - VARS.speed * TIME:GetDeltaTime(), 0 ) );
	end
end