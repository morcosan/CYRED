VARS = {
	speed = "FLOAT"
}


local transform

function OnStart()
	transform = GAMEOBJECT:GetComponent("Transform")
end


function OnUpdate()
	local vec = Vector3( 0, 0, VARS.speed * TIME:GetDeltaTime() )
	print( vec.z )
	print( transform.positionWorld )
	-- print( Vector3( 0, 0, VARS.speed * TIME:GetDeltaTime() ) )
	-- transform:RotateByLocal( Vector3( 0, 0, VARS.speed * TIME:GetDeltaTime() ) )
end
