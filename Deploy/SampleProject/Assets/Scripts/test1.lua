
VARS = {
	dir = "VECTOR3",
	size = "FLOAT"
}

function OnStart()
	print( "OnStart")

	print( GAMEOBJECT )
	print( GAMEOBJECT.name )
	print( GAMEOBJECT:GetComponent("Transform") )

end


function OnUpdate()
end