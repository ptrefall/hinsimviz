#include "WrapPropertyContainer.h"
#include "WrapIProperty.h"
#include "WrapObject.h"
#include "WrapNode.h"
#include "ScriptManager.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Events/IEngineEventManager.h>
#include <Engine/Scene/Object.h>
#include <Engine/Scene/Node.h>
#include <Engine/Events/Event.h>

using namespace Engine;
using namespace Script;
using namespace LuaPlus;

WrapPropertyContainer::WrapPropertyContainer(Core::CoreManager *coreMgr, Script::WrapObject *wObj)
: engineEvents(coreMgr->getEngineEventMgr())
{
	this->coreMgr = coreMgr;
	this->wObj = wObj;
	this->wNode = NULL;
}

WrapPropertyContainer::WrapPropertyContainer(Core::CoreManager *coreMgr, Script::WrapNode *wNode)
: engineEvents(coreMgr->getEngineEventMgr())
{
	this->coreMgr = coreMgr;
	this->wObj = NULL;
	this->wNode = wNode;
}

WrapPropertyContainer::~WrapPropertyContainer()
{
	for(unsigned int i = 0; i < wProperties.size(); i++)
	{
		WrapIProperty *wProp = wProperties[i];
		delete wProp;
		wProp = NULL;
	}
	wProperties.clear();
	lProperties.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
}

int WrapPropertyContainer::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	

	if(wObj)
	{
		lProperties = wObj->getLObj().CreateTable("Properties");
		
		Engine::Scene::Object *obj = wObj->getObj();
		std::map<CL_String, Engine::Entity::IProperty*> &properties = obj->GetProperties();
		std::map<CL_String, Engine::Entity::IProperty*>::iterator propIt = properties.begin();
		for(; propIt != properties.end(); ++propIt)
		{
			WrapIProperty *wProp = new WrapIProperty(coreMgr, wObj, this, propIt->second);
			int fail = wProp->init();
			if(fail)
			{
				delete wProp;
				wProp = NULL;

				CL_String msg = cl_format("Failed to initialize Property Wrapper for property %1", propIt->first);
				coreMgr->getLogMgr()->log("WrapPropertyContainer::Init", msg, Log::L_ERROR);
				continue;
			}
			wProperties.push_back(wProp);
		}

		LuaObject lMeta = wObj->getLMeta();
		lMeta.RegisterDirect("AddProperty", *this, &WrapPropertyContainer::AddProperty);
		
		engineEvents.Connect("PropertyAdded", this, &WrapPropertyContainer::OnPropertyAdded);
	}
	else if(wNode)
	{
		lProperties = wNode->getLNode().CreateTable("Properties");
		
		Engine::Scene::Node *node = wNode->getNode();
		std::map<CL_String, Engine::Entity::IProperty*> &properties = node->GetProperties();
		std::map<CL_String, Engine::Entity::IProperty*>::iterator propIt = properties.begin();
		for(; propIt != properties.end(); ++propIt)
		{
			WrapIProperty *wProp = new WrapIProperty(coreMgr, wNode, this, propIt->second);
			int fail = wProp->init();
			if(fail)
			{
				delete wProp;
				wProp = NULL;
				
				CL_String msg = cl_format("Failed to initialize Property Wrapper for property %1", propIt->first);
				coreMgr->getLogMgr()->log("WrapPropertyContainer::Init", msg, Log::L_ERROR);
				continue;
			}
			wProperties.push_back(wProp);
		}

		LuaObject lMeta = wNode->getLMeta();
		lMeta.RegisterDirect("AddProperty", *this, &WrapPropertyContainer::AddProperty);

		engineEvents.Connect("PropertyAdded", this, &WrapPropertyContainer::OnPropertyAdded);
	}
	else
		return 1;
	return 0;
}

void WrapPropertyContainer::AddProperty(LuaObject self, LuaObject lName, LuaObject defValue)
{
	if(!self.IsTable())
	{
		CL_String self_type = self.TypeName();
		
		CL_String msg = cl_format("Failed to add property, because the type of self was %1 when expecting Table", self_type);
		coreMgr->getLogMgr()->log("WrapPropertyContainer::AddProperty", msg, Log::L_ERROR);
		return;
	}

	if(!lName.IsString())
	{
		CL_String name_type = lName.TypeName();
		
		CL_String msg = cl_format("Failed to add property, because the type of name was %1 when expecting String", name_type);
		coreMgr->getLogMgr()->log("WrapPropertyContainer::AddProperty", msg, Log::L_ERROR);
		return;
	}

	CL_String name = lName.ToString();
	
	Scene::Node *node = NULL;
	bool isObj = false;
	if(wObj)
	{
		node = static_cast<Scene::Node*>(wObj->getObj());
		isObj = true;
	}
	else if(wNode)
	{
		node = wNode->getNode();
	}

	if(node == NULL)
	{
		return;
	}

	if(defValue.IsBoolean())
	{
		bool val = defValue.GetBoolean();
		node->AddProperty<bool>(name, val);
	}
	else if(defValue.IsNumber())
	{
		float val = (float)defValue.ToNumber();
		node->AddProperty<float>(name, val);
	}
	else if(defValue.IsInteger())
	{
		int val = defValue.ToInteger();
		node->AddProperty<int>(name, val);
	}
	else if(defValue.IsString())
	{
		CL_String val = defValue.ToString();
		node->AddProperty<CL_String>(name, val);
	}
	else if(defValue.IsTable())
	{
		int indexes = 0;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;
		try
		{
			x = (float)defValue.GetByName("x").ToNumber();
			indexes++;
		}
		catch(const LuaPlus::LuaException &)
		{
		}
		try
		{
			y = (float)defValue.GetByName("y").ToNumber();
			indexes++;
		}
		catch(const LuaPlus::LuaException &)
		{
		}
		try
		{
			z = (float)defValue.GetByName("z").ToNumber();
			indexes++;
		}
		catch(const LuaPlus::LuaException &)
		{
		}
		try
		{
			w = (float)defValue.GetByName("w").ToNumber();
			indexes++;
		}
		catch(const LuaPlus::LuaException &)
		{
		}

		if(indexes == 2)
		{
			CL_Vec2f val = CL_Vec2f(x,y);
			node->AddProperty<CL_Vec2f>(name, val);
		}
		else if(indexes == 3)
		{
			CL_Vec3f val = CL_Vec3f(x,y,z);
			node->AddProperty<CL_Vec3f>(name, val);
		}
		if(indexes == 4)
		{
			CL_Vec4f val = CL_Vec4f(x,y,z,w);
			node->AddProperty<CL_Vec4f>(name, val);
		}
	}
	return;
}

void WrapPropertyContainer::add(const CL_String &name, Entity::IProperty *prop, WrapIProperty *wProp, bool isObj)
{
	if(prop == NULL)
	{
		CL_String msg = cl_format("Failed to add property %1, because no property was returned", name);
		coreMgr->getLogMgr()->log("WrapPropertyContainer::add", msg, Log::L_ERROR);
		return;
	}
	if(isObj)
		wProp = new WrapIProperty(coreMgr, wObj, this, prop);
	else
		wProp = new WrapIProperty(coreMgr, wNode, this, prop);
	int fail = wProp->init();
	if(fail)
	{
		delete wProp;
		wProp = NULL;
		
		CL_String msg = cl_format("Failed to initialize property wrapper for property %1", prop->GetName());
		coreMgr->getLogMgr()->log("WrapPropertyContainer::add", msg, Log::L_ERROR);
		return;
	}
	wProperties.push_back(wProp);
}

void WrapPropertyContainer::OnPropertyAdded(const Engine::Events::EngineEvent &event)
{
	Engine::Scene::Node *node = event.getArgument(1).ToNode();
	bool isObj = false;
	if(wObj)
	{
		isObj = true;
		if(node->getId() != wObj->getObj()->getId())
			return;
	}
	else if(wNode)
	{
		if(node->getId() != wNode->getNode()->getId())
			return;
	}
	else
		return;

	Engine::Entity::IProperty *prop = event.getArgument(0).ToProperty();
	WrapIProperty *wProp = NULL;
	add(prop->GetName(), prop, wProp, isObj);
}
