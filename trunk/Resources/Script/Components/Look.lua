
if(Look == nil) then
	Look = {}
end

function Look:OnInit(go)
	Log("Look:OnInit", "init Look for " .. go.name, LOG_INFO)
	go:AddProperty("LookVelocity", 0.2)
end

function Look:OnUpdate(go, dt)
	local rel = Human:GetRelativeFocus()
	if(rel.x ~= nil and rel.x ~= 0.0) then
		go:Rotate(rel.x*go:GetLookVelocity(), {x = 0.0, y = -1.0, z = 0.0} )
	end
	
	if(rel.y ~= nil and rel.y ~= 0.0) then
		go:Rotate(rel.y*go:GetLookVelocity(), {x = -1.0, y = 0.0, z = 0.0} )
	end
end

RegisterComponent("Look")
