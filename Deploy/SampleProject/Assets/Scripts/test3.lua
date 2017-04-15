VARS = {
	prefab = "PREFAB"
}


function OnStart()
	SCENE:Instantiate( VARS.prefab, 0 )
end