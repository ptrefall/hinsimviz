#pragma once

#include <Engine/Scene/Node.h>
#include <Depends/Entity/Property.h>

namespace Engine 
{ 
namespace Core { class CoreManager; }

namespace Nodes
{
	class Plane : public Engine::Scene::Node
	{
	public:
		virtual ~Plane();

		static const char *getSType() { return "Plane"; }
		static Engine::Scene::Node *create(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Engine::Core::CoreManager *coreMgr) { return new Plane(id, colorId, type, name, coreMgr); }
		virtual CL_String getNodeType() { return getSType(); }

		virtual int init();

	protected:
		Plane(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Engine::Core::CoreManager *coreMgr);
		virtual void localDisplay();

		void initIndices();
		void initVertices();
		void initTexCoords();

		std::vector<unsigned int> indices;
		std::vector<float> vertices;
		std::vector<float> texCoords;

		Engine::Entity::Property<CL_Vec3f> size_property;
	};
}}
