
Print("Test's init script loading")

if(Test == nil) then
	Test = {}
	Print("Test table generated")
end

function Test:init()
	
	local playerVehicle = Objects[6]
	if(playerVehicle ~= nil) then
		Human:AddObject(playerVehicle)
		Human:SendEvent("MountWeapon", "Chaingun")
		Human:SendEvent("MountEngine", "ThrustEngine")
		local cam = CreateObject("ChaseCamera")
		playerVehicle:Add(cam)
		Human:AddObject(cam)
		cam:SetPosition({x=0.0, y=100.0, z=-200.0})
		ActivateCamera(cam)
	end
	
	--[[local cam = CreateObject("FPSCamera")
	Human:AddObject(cam)
	ActivateCamera(cam)--]]
	
	Print("Test initialized in Lua")
end

Print("Test's init script finished loading")
