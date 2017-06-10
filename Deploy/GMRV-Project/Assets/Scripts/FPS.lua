

local text3d

function OnStart()
	text3d = GAMEOBJECT:GetComponent_Text3D()
end


function OnUpdate()
	text3d.text = string.format( "%.2f", 1.0 / TIME:GetRenderDeltaTime() )
end