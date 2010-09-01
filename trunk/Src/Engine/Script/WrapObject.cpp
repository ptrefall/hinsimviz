#include "WrapObject.h"
#include "WrapObjectManager.h"
#include "WrapPropertyContainer.h"
#include "WrapComponentContainer.h"
#include "WrapNode.h"
#include "WrapSceneManager.h"
#include "ScriptManager.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Scene/ObjectManager.h>
#include <Engine/Scene/Object.h>
#include <Engine/Scene/Node.h>
#include <Engine/Events/Event.h>

using namespace Engine;
using namespace Script;
using namespace LuaPlus;

WrapObject::WrapObject(Core::CoreManager *coreMgr, WrapObjectManager *wObjMgr, Scene::Object *obj)
: wPropContainer(NULL), wCompContainer(NULL)
{
	this->coreMgr = coreMgr;
	this->wObjMgr = wObjMgr;
	this->obj = obj;
}

WrapObject::~WrapObject()
{
	if(wPropContainer)
	{
		delete wPropContainer;
		wPropContainer = NULL;
	}
	if(wCompContainer)
	{
		delete wCompContainer;
		wCompContainer = NULL;
	}

	if(wNode)
	{
		coreMgr->getScriptMgr()->getWSceneMgr()->removeWNode(wNode);
		delete wNode;
		wNode = NULL;
	}

	lObj.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
}

int WrapObject::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	unsigned int objId = obj->getId();
	
	LuaObject &lObjs = wObjMgr->getLObjs();
	lObj = lObjs.CreateTable((int)objId);
	lObj.SetInteger("id", (int)objId);
	lObj.SetString("type", obj->getType().c_str());
	lObj.SetString("name", obj->getName().c_str());
	lMeta = lObj.CreateTable("MetaTable");
	lMeta.SetObject("__index", lMeta);

	lMeta.RegisterDirect("Add", *this, &WrapObject::Add);
	lMeta.RegisterDirect("SendEvent", *this, &WrapObject::SendEvent);
	lMeta.RegisterDirect("SendCommand", *this, &WrapObject::SendCommand);
	lMeta.RegisterDirect("Init", *this, &WrapObject::Init);
	lMeta.RegisterDirect("Translate", *this, &WrapObject::Translate);
	lMeta.RegisterDirect("Kill", *this, &WrapObject::Kill);
	lMeta.RegisterDirect("Rotate", *this, &WrapObject::Rotate);
	lMeta.RegisterDirect("GetParent", *this, &WrapObject::GetParent);
	lMeta.RegisterDirect("FindNode", *this, &WrapObject::FindNode);
	
	lObj.SetLightUserData("__object", this);
	lObj.SetMetaTable(lMeta);

	wPropContainer = new WrapPropertyContainer(coreMgr, this);
	int fail = wPropContainer->init();
	if(fail)
	{
		delete wPropContainer;
		wPropContainer = NULL;
		
		CL_String err = cl_format("Failed to initialize property container wrapper for IObject wrapper with id: %1", objId);
		coreMgr->getLogMgr()->log("WrapObject::init", err, Log::L_ERROR);
		return 1;
	}

	wCompContainer = new WrapComponentContainer(coreMgr, this);
	fail = wCompContainer->init();
	if(fail)
	{
		delete wCompContainer;
		wCompContainer = NULL;
		
		CL_String err = cl_format("Failed to initialize component container wrapper for IObject wrapper with id: %1", objId);
		coreMgr->getLogMgr()->log("WrapObject::init", err, Log::L_ERROR);
		return 1;
	}

	wNode = new WrapNode(coreMgr, coreMgr->getScriptMgr()->getWSceneMgr(), static_cast<Scene::Node*>(obj));
	fail = wNode->init();
	if(fail)
	{
		delete wNode;
		wNode = NULL;

		CL_String err = cl_format("Failed to initialize INode wrapper for IObject wrapper with id: %1", objId);
		coreMgr->getLogMgr()->log("WrapObject::init", err, Log::L_ERROR);
		return 1;
	}
	coreMgr->getScriptMgr()->getWSceneMgr()->addWNode(wNode);

	return 0;
}

void WrapObject::Add(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lNode)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::Add", msg, Log::L_ERROR);
		return;
	}

	if(!lNode.IsTable() && !lNode.IsInteger())
	{
		CL_String msg = cl_format("Node was not a table, nor an integer, (it's a %1)", lNode.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::Add", msg, Log::L_ERROR);
		return;
	}

	LuaObject lNodeId = lNode.GetByName("id");
	if(!lNodeId.IsInteger())
	{
		CL_String msg = cl_format("NodeId was not an integer, (it's a %1)", lNodeId.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::Add", msg, Log::L_ERROR);
		return;
	}

	int nodeId = lNodeId.ToInteger();
	WrapNode *wN = coreMgr->getScriptMgr()->getWSceneMgr()->getWNode(nodeId);
	Scene::Node *n = wN->getNode();
	if(n->getParent())
		n->getParent()->remove(n, false);

	obj->add(n);
}

void WrapObject::SendCommand(LuaObject lSelf, LuaObject lCommand)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::SendCommand", msg, Log::L_ERROR);
		return;
	}

	if(!lCommand.IsString())
	{
		CL_String msg = cl_format("CommandType was not a string (it's a %1)", lCommand.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::SendCommand", msg, Log::L_ERROR);
		return;
	}

	obj->executeCommand(lCommand.ToString(), NULL);
}

void WrapObject::SendEvent(LuaObject lSelf, LuaObject lEventType, LuaObject lEventArg)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::SendEvent", msg, Log::L_ERROR);
		return;
	}

	if(!lEventType.IsString())
	{
		CL_String msg = cl_format("EventType was not a string (it's a %1)", lEventType.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::SendEvent", msg, Log::L_ERROR);
		return;
	}

	if(!lEventArg.IsString() && !lEventArg.IsBoolean() && !lEventArg.IsNumber() && !lEventArg.IsTable() && !lEventArg.IsNil())
	{
		CL_String msg = cl_format("EventArg was not a string, bool, number, table or nil (it's a %1)", lEventArg.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::SendEvent", msg, Log::L_ERROR);
		return;
	}

	CL_String eventType = lEventType.ToString();
	if(lEventArg.IsString())
	{
		CL_String arg = lEventArg.ToString();
		obj->executeEvent(Engine::Events::Event<CL_String, Engine::Scene::Object*>(eventType.c_str(), arg, obj), NULL);
	}
	else if(lEventArg.IsBoolean())
	{
		bool arg = lEventArg.GetBoolean();
		obj->executeEvent(Engine::Events::Event<bool, Engine::Scene::Object*>(eventType.c_str(), arg, obj), NULL);
	}
	else if(lEventArg.IsNumber())
	{
		float arg = lEventArg.GetFloat();
		obj->executeEvent(Engine::Events::Event<float, Engine::Scene::Object*>(eventType.c_str(), arg, obj), NULL);
	}
	else if(lEventArg.IsTable())
	{	
		LuaObject xObj = lEventArg.GetByName("x");
		LuaObject yObj = lEventArg.GetByName("y");
		LuaObject zObj = lEventArg.GetByName("z");

		CL_Vec3f arg((float)xObj.ToNumber(), (float)yObj.ToNumber(), (float)zObj.ToNumber());
		obj->executeEvent(Engine::Events::Event<CL_Vec3f, Engine::Scene::Object*>(eventType.c_str(), arg, obj), NULL);
	}
	else
	{
		obj->executeEvent(Engine::Events::Event<Engine::Scene::Object*, Engine::Scene::Object*>(eventType.c_str(), obj, obj), NULL);
	}
}

void WrapObject::Init(LuaObject lSelf)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::Init", msg, Log::L_ERROR);
		return;
	}

	obj->init();
}

void WrapObject::Translate(LuaObject lSelf, LuaObject lX, LuaObject lY, LuaObject lZ)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::Translate", msg, Log::L_ERROR);
		return;
	}

	if(!lX.IsNumber())
	{
		CL_String msg = cl_format("X was not a number (it's a %1)", lX.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::Translate", msg, Log::L_ERROR);
		return;
	}
	if(!lY.IsNumber())
	{
		CL_String msg = cl_format("Y was not a number (it's a %1)", lY.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::Translate", msg, Log::L_ERROR);
		return;
	}
	if(!lZ.IsNumber())
	{
		CL_String msg = cl_format("Z was not a number (it's a %1)", lZ.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::Translate", msg, Log::L_ERROR);
		return;
	}

	obj->translate(lX.GetFloat(), lY.GetFloat(), lZ.GetFloat());
}

void WrapObject::Kill(LuaPlus::LuaObject lSelf)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::Kill", msg, Log::L_ERROR);
		return;
	}

	CL_String msg = cl_format("Registering object %1 of type %2 for kill!", obj->getName(), obj->getType());
	coreMgr->getLogMgr()->log("WrapObject::Kill", msg, Log::L_INFO);
	obj->kill();
}

void WrapObject::Rotate(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lVal, LuaPlus::LuaObject lAxis)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::Rotate", msg, Log::L_ERROR);
		return;
	}

	if(!lVal.IsNumber())
	{
		CL_String msg = cl_format("Value was not a number (it's a %1)", lVal.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::Rotate", msg, Log::L_ERROR);
		return;
	}

	if(!lAxis.IsTable())
	{
		CL_String msg = cl_format("Axis was not a table (it's a %1)", lAxis.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::Rotate", msg, Log::L_ERROR);
		return;
	}

	float val = (float)lVal.ToNumber();
	float x = (float)lAxis.GetByName("x").ToNumber();
	float y = (float)lAxis.GetByName("y").ToNumber();
	float z = (float)lAxis.GetByName("z").ToNumber();

	obj->rotate(val, CL_Vec3f(x,y,z));
}

LuaObject WrapObject::GetParent(LuaObject lSelf)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::GetParent", msg, Log::L_ERROR);
		return LuaObject();
	}

	if(!obj->getParent())
	{
		return LuaObject();
	}
	
	WrapObject * wParent = wObjMgr->getWObj(obj->getParent()->getId());
	if(wParent == NULL)
	{
		return LuaObject();
	}

	return wParent->getLObj();
}

LuaObject WrapObject::FindNode(LuaObject lSelf, LuaObject lType)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::FindNode", msg, Log::L_ERROR);
		return LuaObject();
	}

	if(!lType.IsString())
	{
		CL_String msg = cl_format("Type was not a string (it's a %1)", lType.TypeName());
		coreMgr->getLogMgr()->log("WrapObject::FindNode", msg, Log::L_ERROR);
		return LuaObject();
	}

	CL_String type = lType.ToString();
	Engine::Scene::Node *typeNode = obj->findNode(type);
	if(typeNode == NULL)
	{
		CL_String msg = cl_format("Could NOT find node of type %1", type);
		coreMgr->getLogMgr()->log("WrapObject::FindNode", msg, Log::L_ERROR);
		return LuaObject();
	}

	WrapNode *wTypeNode = coreMgr->getScriptMgr()->getWSceneMgr()->getWNode(typeNode->getId());
	if(wTypeNode == NULL)
	{
		CL_String msg = cl_format("Node of type %1 has not been wrapped to Lua!", type);
		coreMgr->getLogMgr()->log("WrapObject::FindNode", msg, Log::L_ERROR);
		return LuaObject();
	}
	return wTypeNode->getLNode();
}
