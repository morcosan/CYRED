
VARS = {
	dir = "VECTOR3",
	size = "FLOAT"
}

a
function OnStart()
	print( "OnStart")

	print( GAMEOBJECT )
	print( GAMEOBJECT.name )
	print( GAMEOBJECT:GetComponent("Transform") )

end


function OnUpdate()
end