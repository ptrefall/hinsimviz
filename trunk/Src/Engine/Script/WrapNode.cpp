#include "WrapNode.h"
#include "WrapSceneManager.h"
#include "WrapPropertyContainer.h"
#include "ScriptManager.h"
#include <Engine/Core/CoreManager.h>
#include <Engine/Log/LogManager.h>
#include <Engine/Scene/SceneManager.h>
#include <Engine/Scene/Node.h>
#include <Engine/Scene/Object.h>
#include <Engine/Events/Event.h>
#include <Engine/Resource/ResManager.h>

using namespace Engine;
using namespace Script;
using namespace LuaPlus;

WrapNode::WrapNode(Core::CoreManager *coreMgr, WrapSceneManager *wSceneMgr, Scene::Node *node)
{
	this->coreMgr = coreMgr;
	this->wSceneMgr = wSceneMgr;
	this->node = node;
}

WrapNode::~WrapNode()
{
	if(wPropContainer)
	{
		delete wPropContainer;
		wPropContainer = NULL;
	}

	lNode.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
}

int WrapNode::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	unsigned int nodeId = node->getId();
	
	LuaObject &lNodes = wSceneMgr->getLNodes();
	lNode = lNodes.CreateTable((int)nodeId);
	lNode.SetInteger("id", (int)nodeId);
	lNode.SetString("type", node->getType().c_str());
	lNode.SetString("name", node->getName().c_str());
	lMeta = lNode.CreateTable("MetaTable");
	lMeta.SetObject("__index", lMeta);

	lMeta.RegisterDirect("Add", *this, &WrapNode::Add);
	lMeta.RegisterDirect("SendCommand", *this, &WrapNode::SendCommand);
	lMeta.RegisterDirect("SendEvent", *this, &WrapNode::SendEvent);
	lMeta.RegisterDirect("Init", *this, &WrapNode::Init);
	lMeta.RegisterDirect("Translate", *this, &WrapNode::Translate);
	lMeta.RegisterDirect("SetTexture", *this, &WrapNode::SetTexture);
	lMeta.RegisterDirect("SetShader", *this, &WrapNode::SetShader);
	lMeta.RegisterDirect("Kill", *this, &WrapNode::Kill);
	lMeta.RegisterDirect("Rotate", *this, &WrapNode::Rotate);
	lMeta.RegisterDirect("FindNode", *this, &WrapNode::FindNode);

	lNode.SetLightUserData("__object", this);
	lNode.SetMetaTable(lMeta);

	wPropContainer = new WrapPropertyContainer(coreMgr, this);
	int fail = wPropContainer->init();
	if(fail)
	{
		delete wPropContainer;
		wPropContainer = NULL;
		
		CL_String err = cl_format("Failed to initialize property container wrapper for INode wrapper with id: %1", nodeId);
		coreMgr->getLogMgr()->log("WrapNode::init", err, Log::L_ERROR);
		return 1;
	}

	return 0;
}

void WrapNode::Add(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lNode)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::Add", msg, Log::L_ERROR);
		return;
	}

	if(!lNode.IsTable() && !lNode.IsInteger())
	{
		CL_String msg = cl_format("Node was not a table, nor an integer, (it's a %1)", lNode.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::Add", msg, Log::L_ERROR);
		return;
	}

	LuaObject lNodeId = lNode.GetByName("id");
	if(!lNodeId.IsInteger())
	{
		CL_String msg = cl_format("NodeId was not an integer, (it's a %1)", lNodeId.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::Add", msg, Log::L_ERROR);
		return;
	}

	int nodeId = lNodeId.ToInteger();
	WrapNode *wN = wSceneMgr->getWNode(nodeId);
	Scene::Node *n = wN->getNode();
	if(n->getParent())
		n->getParent()->remove(n, false);

	node->add(n);
}

void WrapNode::SendCommand(LuaObject lSelf, LuaObject lCommand)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::SendCommand", msg, Log::L_ERROR);
		return;
	}

	if(!lCommand.IsString())
	{
		CL_String msg = cl_format("CommandType was not a string (it's a %1)", lCommand.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::SendCommand", msg, Log::L_ERROR);
		return;
	}

	Scene::Object *iObj = node->getAsObject();
	if(iObj == NULL)
	{
		CL_String msg = cl_format("IObject of node with name %1 of type %2 was NULL", node->getName(), node->getType());
		coreMgr->getLogMgr()->log("WrapNode::SendCommand", msg, Log::L_ERROR);
		return;
	}

	iObj->executeCommand(lCommand.ToString(), NULL);
}

void WrapNode::SendEvent(LuaObject lSelf, LuaObject lEventType, LuaObject lEventArg)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::SendEvent", msg, Log::L_ERROR);
		return;
	}

	if(!lEventType.IsString())
	{
		CL_String msg = cl_format("EventType was not a string (it's a %1)", lEventType.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::SendEvent", msg, Log::L_ERROR);
		return;
	}

	if(!lEventArg.IsString() && !lEventArg.IsBoolean() && !lEventArg.IsNumber() && !lEventArg.IsNil())
	{
		CL_String msg = cl_format("EventArg was not a string, bool, number or nil (it's a %1)", lEventArg.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::SendEvent", msg, Log::L_ERROR);
		return;
	}

	Scene::Object *iObj = node->getAsObject();
	if(iObj == NULL)
	{
		CL_String msg = cl_format("IObject of node with name %1 of type %2 was NULL", node->getName(), node->getType());
		coreMgr->getLogMgr()->log("WrapNode::SendEvent", msg, Log::L_ERROR);
		return;
	}

	CL_String eventType = lEventType.ToString();
	if(lEventArg.IsString())
	{
		CL_String arg = lEventArg.ToString();
		iObj->executeEvent(Engine::Events::Event<CL_String, Engine::Scene::Object*>(eventType.c_str(), arg, iObj), NULL);
	}
	else if(lEventArg.IsBoolean())
	{
		bool arg = lEventArg.GetBoolean();
		iObj->executeEvent(Engine::Events::Event<bool, Engine::Scene::Object*>(eventType.c_str(), arg, iObj), NULL);
	}
	else if(lEventArg.IsNumber())
	{
		float arg = lEventArg.GetFloat();
		iObj->executeEvent(Engine::Events::Event<float, Engine::Scene::Object*>(eventType.c_str(), arg, iObj), NULL);
	}
	else
	{
		iObj->executeEvent(Engine::Events::Event<Engine::Scene::Object*, Engine::Scene::Object*>(eventType.c_str(), iObj, iObj), NULL);
	}
}

void WrapNode::Init(LuaObject lSelf)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::Init", msg, Log::L_ERROR);
		return;
	}

	node->init();
}

void WrapNode::Translate(LuaObject lSelf, LuaObject lX, LuaObject lY, LuaObject lZ)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::Translate", msg, Log::L_ERROR);
		return;
	}

	if(!lX.IsNumber())
	{
		CL_String msg = cl_format("X was not a number (it's a %1)", lX.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::Translate", msg, Log::L_ERROR);
		return;
	}
	if(!lY.IsNumber())
	{
		CL_String msg = cl_format("Y was not a number (it's a %1)", lY.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::Translate", msg, Log::L_ERROR);
		return;
	}
	if(!lZ.IsNumber())
	{
		CL_String msg = cl_format("Z was not a number (it's a %1)", lZ.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::Translate", msg, Log::L_ERROR);
		return;
	}

	node->translate(lX.GetFloat(), lY.GetFloat(), lZ.GetFloat());
}

void WrapNode::SetTexture(LuaObject lSelf, LuaObject fileName, LuaObject texIndex, LuaObject uniform)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::SetTexture", msg, Log::L_ERROR);
		return;
	}

	if(!fileName.IsString())
	{
		CL_String msg = cl_format("FileName was not a string (it's a %1)", fileName.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::SetTexture", msg, Log::L_ERROR);
		return;
	}

	if(!texIndex.IsNumber())
	{
		CL_String msg = cl_format("TextureIndex was not a number (it's a %1)", texIndex.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::SetTexture", msg, Log::L_ERROR);
		return;
	}

	if(!uniform.IsString())
	{
		CL_String msg = cl_format("Uniform was not a string (it's a %1)", uniform.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::SetTexture", msg, Log::L_ERROR);
		return;
	}

	node->setTexture(uniform.ToString(), fileName.ToString(), (int)texIndex.ToNumber());
}

void WrapNode::SetShader(LuaObject lSelf, LuaObject fileName)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::SetShader", msg, Log::L_ERROR);
		return;
	}

	if(!fileName.IsString())
	{
		CL_String msg = cl_format("FileName was not a string (it's a %1)", fileName.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::SetShader", msg, Log::L_ERROR);
		return;
	}

	bool success = node->setShader(cl_format("%1/Shader/%2", coreMgr->getResMgr()->getRootPath(), fileName.ToString()));
	if(!success)
	{
		CL_String msg = cl_format("Could not set shader %1 for node %2 of type %3", fileName.ToString(), node->getName(), node->getType());
		coreMgr->getLogMgr()->log("WrapNode::SetShader", msg, Log::L_ERROR);
		return;
	}
}

void WrapNode::Kill(LuaObject lSelf)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::Kill", msg, Log::L_ERROR);
		return;
	}

	CL_String msg = cl_format("Registering node %1 of type %2 for kill!", node->getName(), node->getType());
	coreMgr->getLogMgr()->log("WrapNode::Kill", msg, Log::L_ERROR);
	node->kill();
}

void WrapNode::Rotate(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lVal, LuaPlus::LuaObject lAxis)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::Rotate", msg, Log::L_ERROR);
		return;
	}

	if(!lVal.IsNumber())
	{
		CL_String msg = cl_format("Value was not a number (it's a %1)", lVal.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::Rotate", msg, Log::L_ERROR);
		return;
	}

	if(!lAxis.IsTable())
	{
		CL_String msg = cl_format("Axis was not a table (it's a %1)", lAxis.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::Rotate", msg, Log::L_ERROR);
		return;
	}

	float val = (float)lVal.ToNumber();
	float x = (float)lAxis.GetByName("x").ToNumber();
	float y = (float)lAxis.GetByName("y").ToNumber();
	float z = (float)lAxis.GetByName("z").ToNumber();

	node->rotate(val, CL_Vec3f(x,y,z));
}

LuaObject WrapNode::FindNode(LuaObject lSelf, LuaObject lType)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::FindNode", msg, Log::L_ERROR);
		return LuaObject();
	}

	if(!lType.IsString())
	{
		CL_String msg = cl_format("Type was not a string (it's a %1)", lType.TypeName());
		coreMgr->getLogMgr()->log("WrapNode::FindNode", msg, Log::L_ERROR);
		return LuaObject();
	}

	CL_String type = lType.ToString();
	Engine::Scene::Node *typeNode = node->findNode(type);
	if(typeNode == NULL)
	{
		CL_String msg = cl_format("Could NOT find node of type %1", type);
		coreMgr->getLogMgr()->log("WrapNode::FindNode", msg, Log::L_ERROR);
		return LuaObject();
	}

	WrapNode *wTypeNode = wSceneMgr->getWNode(typeNode->getId());
	if(wTypeNode == NULL)
	{
		CL_String msg = cl_format("Node of type %1 has not been wrapped to Lua!", type);
		coreMgr->getLogMgr()->log("WrapNode::FindNode", msg, Log::L_ERROR);
		return LuaObject();
	}
	return wTypeNode->getLNode();
}
