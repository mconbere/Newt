Feet = getObject("Feet")
Player = getObject("Player")
Jules = getObject("Jules")
Gems = {getObject("gem.0"), getObject("gem.1")}

function onCollision()
	print("Collision!")
end

function onSceneUpdate()
	coll = getNumCollisions(Feet)

	for key, obj in pairs(Gems) do
		if isCollisionBetween(Player, obj) then
			deleteObject(obj)
			table.remove(Gems, key)
		end
	end

	if isKeyPressed("F") then
		asdf()
	end

	move = 0
	if coll > 1 then
		if isKeyPressed("LEFT")  then
			rotate(Player, {0, 0, 1}, 5)
		end
		if isKeyPressed("RIGHT") then
			rotate(Player, {0, 0, 1}, -5)
		end
		if isKeyPressed("UP") then
			move = move + 1
		end
		if isKeyPressed("DOWN") then
			move = move - 1
		end
		
		if isKeyPressed("SPACE") then
			addCentralForce(Player, {0, 0, 15}, "local")
		end
	end

	if move == 0 then
		changeAnimation(Jules, 0)
	else
		addCentralForce(Player, {move * 20, 0, 0}, "local")
		changeAnimation(Jules, 2)
	end

	-- manual friction
	if coll < 2 then
		setLinearDamping(Player, 0.01)
	else
		setLinearDamping(Player, 0.9)
	end	
end
