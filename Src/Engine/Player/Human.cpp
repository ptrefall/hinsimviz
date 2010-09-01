#include "Human.h"
#include <Core/CoreManager.h>
#include <GameState/GameStateManager.h>
#include <GameState/IGameState.h>
#include <Scene/SceneManager.h>
#include <Scene/Camera.h>
#include <Scene/Node.h>
#include <Scene/Object.h>

using namespace Engine;
using namespace Player;

Human::Human(unsigned int id, const CL_String &name, Core::CoreManager *coreMgr)
: IPlayer(id, name, coreMgr), lastHoveredObject(NULL)
{
	this->id = id;
	this->name = name;
	this->coreMgr = coreMgr;
}

Human::~Human()
{
}

void Human::addGO(Scene::Object *go)
{
	gos.push_back(go);
}

void Human::removeGO(Scene::Object *go)
{
	for(unsigned int i = 0; i < gos.size(); i++)
	{
		if(gos[i] == go)
		{
			gos.erase((gos.begin()+i));
			break;
		}
	}
}

Scene::Object *Human::getGO(unsigned int i)
{
	if(i >= gos.size())
		return NULL;
	return gos[i];
}

unsigned int Human::getGOCount() const
{
	return gos.size();
}


void Human::sendCommand(const CL_String &command)
{
	for(unsigned int i = 0; i < gos.size(); i++)
	{
		gos[i]->executeCommand(command, this);
	}

	//Ok, so this is a bit hacky...
	if(gos.empty())
	{
		if(lastHoveredObject && (command == "Clicked" || command == "ClickedEnd"))
		{
			lastHoveredObject->executeCommand(command, this);
		}
	}
}

void Human::sendEvent(const Events::IEvent &event)
{
	for(unsigned int i = 0; i < gos.size(); i++)
	{
		gos[i]->executeEvent(event, this);
	}

	/*if(gos.empty())
	{
		if(lastHoveredObject)
		{
			lastHoveredObject->executeEvent(event, this);
		}
	}*/
}

void Human::hover(const CL_Vec3i &abs, const CL_Vec3i &rel)
{
	focusPoint = CL_Vec3f((float)abs.x, (float)abs.y, (float)abs.z);
	int relX = rel.x;
	int relY = rel.y;
	int relZ = rel.z;

	//Make a little deadspace...
	if(relX <= 1 && relX >= -1)
		relX = 0;
	if(relY <= 1 && relY >= -1)
		relY = 0;
	if(relZ <= 1 && relZ >= -1)
		relZ = 0;
	relativeFocus = CL_Vec3f((float)relX, (float)relY, (float)relZ);

	CL_Vec4f color;
	int result = coreMgr->getSceneMgr()->getCamera()->colorPick(abs, color);
	
	//First check if we hover a gui node
	Scene::Node *guiRoot = coreMgr->getSceneMgr()->getGuiRoot();
	Scene::Node *node = guiRoot->findNode(color);
	if(node)
	{
		if(node->isObject())
		{
			if(lastHoveredObject && lastHoveredObject == node->getAsObject())
				return;

			if(lastHoveredObject)
				lastHoveredObject->executeCommand("HoverEnd", this);

			node->getAsObject()->executeCommand("Hover", this);
			lastHoveredObject = node->getAsObject();
		}
		else if(node->getParent()->isObject())
		{
			if(lastHoveredObject && lastHoveredObject == node->getParent()->getAsObject())
				return;

			if(lastHoveredObject)
				lastHoveredObject->executeCommand("HoverEnd", this);

			node->getParent()->getAsObject()->executeCommand("Hover", this);
			lastHoveredObject = node->getParent()->getAsObject();
		}
		return;
	}

	//Secon, check if we hover a scene node
	Scene::Node *sceneRoot = coreMgr->getSceneMgr()->getRoot();
	node = sceneRoot->findNode(color);
	if(node)
	{
		if(node->isObject())
		{
			if(lastHoveredObject && lastHoveredObject == node->getAsObject())
				return;

			if(lastHoveredObject)
				lastHoveredObject->executeCommand("HoverEnd", this);

			node->getAsObject()->executeCommand("Hover", this);
			lastHoveredObject = node->getAsObject();
		}
		else if(node->getParent() && node->getParent()->isObject())
		{
			if(lastHoveredObject && lastHoveredObject == node->getParent()->getAsObject())
				return;

			if(lastHoveredObject)
				lastHoveredObject->executeCommand("HoverEnd", this);

			node->getParent()->getAsObject()->executeCommand("Hover", this);
			lastHoveredObject = node->getParent()->getAsObject();
		}
		return;
	}
	else
	{
		if(lastHoveredObject)
		{
			lastHoveredObject->executeCommand("HoverEnd", this);
			lastHoveredObject = NULL;
		}
	}
}

CL_Vec3f Human::getFocusPoint()
{
	return focusPoint;
}

CL_Vec3f Human::getRelativeFocus()
{
	return relativeFocus;
}
