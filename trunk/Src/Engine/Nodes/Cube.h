#pragma once

#include <Engine/Scene/Node.h>
#include <Depends/Entity/Property.h>

namespace Engine 
{ 
namespace Core { class CoreManager; }

namespace Nodes
{
	class Cube : public Engine::Scene::Node
	{
	public:
		virtual ~Cube();

		static const char *getSType() { return "Cube"; }
		static Engine::Scene::Node *create(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Engine::Core::CoreManager *coreMgr) { return new Cube(id, colorId, type, name, coreMgr); }
		virtual CL_String getNodeType() { return getSType(); }

		virtual int init();

	protected:
		Cube(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Engine::Core::CoreManager *coreMgr);
		virtual void localDisplay();

		void initIndices();
		void initVertices();
		void initTexCoords();

		std::vector<unsigned int> indices;
		std::vector<float> vertices;
		std::vector<float> texCoords;
	};
}}
