/*Component-based Entity Engine
Copyright (c) 2009 P�l Trefall and Kenneth Gangst�

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Note: Some of the libraries Component-based Entity Engine may link to may have additional
requirements or restrictions.
*/

#pragma once

#include "precomp.h"
#include <ClanLib/core.h>
#include <Engine/Events/CommandHandler.h>
#include <Engine/Events/IEvent.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Player { class IPlayer; }
namespace Entity
{
class Component;
class ComponentFactory;
class IProperty;

class ComponentContainer
{
public:
	ComponentContainer(ComponentFactory& factory);
	virtual ~ComponentContainer();

	Engine::Core::CoreManager *getCoreMgr() const;

	Component* AddComponent(const CL_String& componentType, const CL_String& componentName = CL_String());
	bool HasComponent(const CL_String& componentName);
	Component* GetComponent(const CL_String& componentName);
	std::vector<Component*> &GetComponents() { return components; }

	void UpdateComponents(double deltaTime);
	void InterpolateComponents(double deltaTime);

	void ExecuteCommandOnComponents(const CL_String &command, Engine::Player::IPlayer *player);
	void ExecuteEventOnComponents(const Events::IEvent &event, Engine::Player::IPlayer *player);

protected:
	std::vector<Component*> components;

	ComponentFactory& factory;
};

}}
