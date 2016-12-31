VARS = {
	speed = "VECTOR3"
}


local transform

function OnStart()
	transform = GAMEOBJECT:GetComponent_Transform()
end


function OnUpdate()
	local rotation = Vector3( 0, 0, 0 )
	rotation.x = VARS.speed.x * TIME:GetDeltaTime()
	rotation.y = VARS.speed.y * TIME:GetDeltaTime()
	rotation.z = VARS.speed.z * TIME:GetDeltaTime()
	transform:RotateByLocal( rotation )

	print( VARS.speed.y )
end
