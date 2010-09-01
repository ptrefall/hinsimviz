#pragma once

#include <Engine/Scene/Node.h>
#include <Depends/Entity/Property.h>

namespace Engine 
{ 
namespace Core { class CoreManager; }
namespace Resource { class IResource; }

namespace Nodes
{
	class Flow3D : public Engine::Scene::Node
	{
	public:
		virtual ~Flow3D();

		static const char *getSType() { return "Flow3D"; }
		static Engine::Scene::Node *create(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Engine::Core::CoreManager *coreMgr) { return new Flow3D(id, colorId, type, name, coreMgr); }
		virtual CL_String getNodeType() { return getSType(); }

		virtual int init();

	protected:
		Flow3D(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Engine::Core::CoreManager *coreMgr);
		virtual void localDisplay();

		void initIndices();
		void initVertices();
		void initTexCoords();

		std::vector<unsigned int> indices;
		std::vector<float> vertices;
		std::vector<float> texCoords;

		Engine::Entity::Property<CL_String> file_property;
		CL_Slot slotFileChanged;
		void OnFileChanged(const CL_String &oldValue, const CL_String &newValue);
		Resource::IResource *bin;
	};
}}
