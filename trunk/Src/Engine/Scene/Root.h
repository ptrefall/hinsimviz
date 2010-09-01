#pragma once

#include "Node.h"
#include <Depends/Entity/Property.h>

namespace Engine
{
namespace Core { class CoreManager; }
namespace Scene
{
	class Root : public Node
	{
	public:
		virtual ~Root();

		static const char *getSType() { return "Root"; }
		static Node *create(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Engine::Core::CoreManager *coreMgr) { return new Root(id, colorId, type, name, coreMgr); }
		virtual CL_String getNodeType() { return getSType(); }

		virtual int init();

	protected:
		Root(unsigned int id, const CL_Vec4f &colorId, const CL_String &type, const CL_String &name, Engine::Core::CoreManager *coreMgr);
	};
}}
