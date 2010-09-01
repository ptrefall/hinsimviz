
if(Thrust == nil) then
	Thrust = {}
end

function Thrust:OnInit(go)
	Log("Thrust:OnInit", "init Thrust for " .. go.name, LOG_INFO)
	go:AddProperty("ThrustForce", 1000.0)
end

function Thrust:OnCommand(go, command)
	local body = go:GetParent()
	if(body == nil) then
		return
	end
	
	local thrustForce = go:GetThrustForce()
	local forceVec = nil
	if(command == "MoveForward") then
		forceVec = {x=0, y=0, z = -thrustForce}
	elseif(command == "MoveBackward") then
		forceVec = {x=0, y=0, z = -(-thrustForce)}
	elseif(command == "MoveUp") then
		forceVec = {x=0, y = -(-thrustForce), z=0}
	elseif(command == "MoveDown") then
		forceVec = {x=0, y = -thrustForce, z=0}
	elseif(command == "MoveLeft") then
		forceVec = {x = -thrustForce, y=0, z=0}
	elseif(command == "MoveRight") then
		forceVec = {x = -(-thrustForce), y=0, z=0}
	elseif(command == "TurnLeft") then
		body:Rotate(1.0, {x=0.0, y=1.0, z=0.0})
	elseif(command == "TurnRight") then
		body:Rotate(-1.0, {x=0.0, y=1.0, z=0.0})
	end
	
	if(forceVec ~= nil) then
		body:SendEvent("ForceActingOnBody", forceVec)
	end
end

RegisterComponent("Thrust")
