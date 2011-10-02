-- Scene script for Sandbox level
-- Author: Brett McLarnon <brett.mclarnon@gmail.com>

Player = getObject("Player")
Feet = getObject("Feet")

function onSceneUpdate()
  coll = getNumCollisions(Feet)

  move = false
  if isKeyPressed("LEFT") and (coll > 1) then
	rotate(Player, {0, 0, 1}, 5)
	move = true
  end
  if isKeyPressed("RIGHT") and (coll > 1) then
	rotate(Player, {0, 0, 1}, -5)
	move = true
  end
  if isKeyPressed("UP") and (coll > 1) then
	move = true
  end
  if isKeyPressed("SPACE") and (coll > 1) then
	addCentralForce(Player, {0, 0, 20}, "local")
  end
  if move then
	addCentralForce(Player, {0, -10, 0}, "local")
	-- changeAnimation(...)
  end

  -- manual friction (I don't understand)
  if coll < 2 then
	setLinearDamping(Player, 0.01)
  else
	setLinearDamping(Player, 0.9)
  end
end
