
Print("Test's init script loading")

if(Test == nil) then
	Test = {}
	Print("Test table generated")
end

function Test:init()
	
	local cam = CreateObject("FPSCamera")
	Human:AddObject(cam)
	ActivateCamera(cam)
	
	local cube = CreateObject("SpinningCube")
	cube:Translate(0.0, 0.0, -100.0)
	Print("Test initialized in Lua")
end

Print("Test's init script finished loading")
