
if(IdleSpin == nil) then
	IdleSpin = {}
end

function IdleSpin:OnInit(go)
	Log("IdleSpin:OnInit", "init IdleSpin for " .. go.name, LOG_INFO)
	go:AddProperty("SpinVelocity", 6.0)
	go:AddProperty("SpinDirection", {x=0.0, y=1.0, z=0.0})
end

function IdleSpin:OnUpdate(go, dt)
	go:Rotate(go:GetSpinVelocity() * dt, go:GetSpinDirection())
end

RegisterComponent("IdleSpin")
