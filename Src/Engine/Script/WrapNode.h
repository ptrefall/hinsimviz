#pragma once

#include <Lua/LuaPlus.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Scene { class Node; }
namespace Script
{
class WrapSceneManager;
class WrapPropertyContainer;

class WrapNode
{
public:
	WrapNode(Core::CoreManager *coreMgr, WrapSceneManager *wSceneMgr, Scene::Node *node);
	~WrapNode();

	int init();

	Scene::Node *getNode() const { return node; }
	LuaPlus::LuaObject getLNode() const { return lNode; }
	LuaPlus::LuaObject getLMeta() const { return lMeta; }

private:
	void Add(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lNode);
	void SendCommand(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lCommand);
	void SendEvent(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lEventType, LuaPlus::LuaObject lEventArg);
	void Init(LuaPlus::LuaObject lSelf);
	void Translate(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lX, LuaPlus::LuaObject lY, LuaPlus::LuaObject lZ);
	void SetTexture(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject fileName, LuaPlus::LuaObject texIndex, LuaPlus::LuaObject uniform);
	void SetShader(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject fileName);
	void Kill(LuaPlus::LuaObject lSelf);
	void Rotate(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lVal, LuaPlus::LuaObject lAxis);
	LuaPlus::LuaObject FindNode(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lType);

	Core::CoreManager *coreMgr;
	Script::WrapSceneManager *wSceneMgr;

	Scene::Node *node;
	LuaPlus::LuaObject lNode;
	LuaPlus::LuaObject lMeta;
	WrapPropertyContainer *wPropContainer;
};

}
}
