

function OnCollisionEnter( other, isTrigger )
	-- if other.tag == "Destructable" then
	-- 	SCENE:Destroy( other )
	-- end

	print( other.name )
	print( isTrigger )
end

