#pragma once

#include <Lua/LuaPlus.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Scene { class Object; }
namespace Script
{
class WrapObjectManager;
class WrapPropertyContainer;
class WrapComponentContainer;
class WrapNode;

class WrapObject
{
public:
	WrapObject(Core::CoreManager *coreMgr, WrapObjectManager *wObjMgr, Scene::Object *obj);
	~WrapObject();

	int init();

	Scene::Object *getObj() const { return obj; }
	LuaPlus::LuaObject getLObj() const { return lObj; }
	LuaPlus::LuaObject getLMeta() const { return lMeta; }

protected:
	void Add(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lNode);
	void SendCommand(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lCommand);
	void SendEvent(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lEventType, LuaPlus::LuaObject lEventArg);
	void Init(LuaPlus::LuaObject lSelf);
	void Translate(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lX, LuaPlus::LuaObject lY, LuaPlus::LuaObject lZ);
	void Kill(LuaPlus::LuaObject lSelf);
	void Rotate(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lVal, LuaPlus::LuaObject lAxis);
	LuaPlus::LuaObject GetParent(LuaPlus::LuaObject lSelf);
	LuaPlus::LuaObject FindNode(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lType);

	Core::CoreManager *coreMgr;
	Script::WrapObjectManager *wObjMgr;

	Scene::Object *obj;
	LuaPlus::LuaObject lObj;
	LuaPlus::LuaObject lMeta;
	WrapPropertyContainer *wPropContainer;
	WrapComponentContainer *wCompContainer;
	WrapNode *wNode;
};

}
}
