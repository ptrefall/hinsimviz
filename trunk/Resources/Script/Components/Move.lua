
if(Move == nil) then
	Move = {}
end

function Move:OnInit(go)
	Log("Move:OnInit", "init Move for " .. go.name, LOG_INFO)
	go:AddProperty("Velocity", 1.0)
	go:AddProperty("MoveDirection", {x=0.0, y=0.0, z=0.0})
end

function Move:OnCommand(go, command)
	local moveDirection = { x=0.0, y=0.0, z=0.0 }
	if(command == "MoveForward") then
		moveDirection.z = moveDirection.z - velocity_property.Get();
	elseif(command == "MoveForwardEnd") then
		moveDirection.z = 0.0;
	elseif(command == "MoveBackward") then
		moveDirection.z = moveDirection.z + velocity_property.Get();
	elseif(command == "MoveBackwardEnd") then
		moveDirection.z = 0.0;
	elseif(command == "MoveUp") then
		moveDirection.y = moveDirection.y + velocity_property.Get();
	elseif(command == "MoveUpEnd") then
		moveDirection.y = 0.0;
	elseif(command == "MoveDown") then
		moveDirection.y = moveDirection.y - velocity_property.Get();
	elseif(command == "MoveDownEnd") then
		moveDirection.y = 0.0;
	elseif(command == "MoveLeft") then
		moveDirection.x = moveDirection.x - velocity_property.Get();
	elseif(command == "MoveLeftEnd") then
		moveDirection.x = 0.0;
	elseif(command == "MoveRight") then
		moveDirection.x = moveDirection.x + velocity_property.Get();
	elseif(command == "MoveRightEnd") then
		moveDirection.x = 0.0;
	end
	
	if(moveDirection.x ~= 0.0 or moveDirection.y ~= 0.0 or moveDirection.z ~= 0.0) then
		local moveDir = go:GetMoveDirection()
		--go:SetMoveDirection({ x = moveDir.x + moveDirection.x,
		--					  y = moveDir.y + moveDirection.y,
		--					  z = moveDir.z + moveDirection.z })
	end
end

function Move:OnUpdate(go, dt)
	local moveDirection = go:GetMoveDirection()
	if(moveDirection.x ~= 0.0 or moveDirection.y ~= 0.0 or moveDirection.z ~= 0.0) then
		go:Translate(moveDirection)
	end
end

RegisterComponent("Move")
