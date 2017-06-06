

function OnCollisionEnter( other )
	if other.tag == "Destructable" then
		SCENE:Destroy( other )
	end
end

