#pragma once

#include <Depends/Entity/ComponentFactory.h>

/*#include <Game/ComponentManager/Health.h>
#include <Game/ComponentManager/Gun.h>
#include <Game/ComponentManager/Explodable.h>
#include <Game/ComponentManager/IdleSpin.h>
#include <Game/ComponentManager/Move.h>
#include <Game/ComponentManager/Clickable.h>
#include <Game/ComponentManager/Button.h>
#include <Game/ComponentManager/Selectable.h>
#include <Game/ComponentManager/Shape.h>
#include <Game/ComponentManager/Dragable.h>
#include <Game/ComponentManager/WeaponMount.h>
#include <Game/ComponentManager/EngineMount.h>
#include <Game/ComponentManager/Physical.h>*/

namespace Engine
{
namespace Scene
{
	class ComponentRegistrator
	{
	public:
		static void Register(Entity::ComponentFactory *factory)
		{
			/*factory->RegisterComponent(Game::Component::Health::GetType(), &Game::Component::Health::Create);
			factory->RegisterComponent(Game::Component::Gun::GetType(), &Game::Component::Gun::Create);
			factory->RegisterComponent(Game::Component::Explodable::GetType(), &Game::Component::Explodable::Create);
			factory->RegisterComponent(Game::Component::IdleSpin::GetType(), &Game::Component::IdleSpin::Create);
			factory->RegisterComponent(Game::Component::Move::GetType(), &Game::Component::Move::Create);
			factory->RegisterComponent(Game::Component::Clickable::GetType(), &Game::Component::Clickable::Create);
			factory->RegisterComponent(Game::Component::Button::GetType(), &Game::Component::Button::Create);
			factory->RegisterComponent(Game::Component::Selectable::GetType(), &Game::Component::Selectable::Create);
			factory->RegisterComponent(Game::Component::Shape::GetType(), &Game::Component::Shape::Create);
			factory->RegisterComponent(Game::Component::Dragable::GetType(), &Game::Component::Dragable::Create);
			factory->RegisterComponent(Game::Component::WeaponMount::GetType(), &Game::Component::WeaponMount::Create);
			factory->RegisterComponent(Game::Component::EngineMount::GetType(), &Game::Component::EngineMount::Create);
			factory->RegisterComponent(Game::Component::Physical::GetType(), &Game::Component::Physical::Create);*/
		}
	};
}}