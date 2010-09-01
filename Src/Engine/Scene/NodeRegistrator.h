#pragma once

#include "NodeFactory.h"
#include <ClanLib/core.h>

#include "Root.h"
#include "Camera.h"
#include <Engine/Nodes/Flow3D.h>
#include <Engine/Nodes/Plane.h>
/*#include <Game/NodeManager/Cube.h>
#include <Game/NodeManager/Sphere.h>
#include <Game/NodeManager/Grid.h>
#include <Game/NodeManager/Plane.h>
#include <Game/NodeManager/Text.h>*/
namespace Engine
{
namespace Scene
{

	class NodeRegistrator
	{
	public:
		static void Register(NodeFactory *factory)
		{
			factory->registerNode(Root::getSType(), &Root::create);
			factory->registerNode(Camera::getSType(), &Camera::create);

			factory->registerNode(Nodes::Plane::getSType(), &Nodes::Plane::create);
			factory->registerNode(Nodes::Flow3D::getSType(), &Nodes::Flow3D::create);

			/*factory->registerNode(Game::Node::Cube::getSType(), &Game::Node::Cube::create);
			factory->registerNode(Game::Node::Sphere::getSType(), &Game::Node::Sphere::create);
			factory->registerNode(Game::Node::Grid::getSType(), &Game::Node::Grid::create);
			factory->registerNode(Game::Node::Plane::getSType(), &Game::Node::Plane::create);
			factory->registerNode(Game::Node::Text::getSType(), &Game::Node::Text::create);*/
		}
	};

}
}
