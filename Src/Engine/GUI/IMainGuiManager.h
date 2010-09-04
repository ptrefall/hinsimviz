#pragma once

namespace Engine
{
namespace Scene { class Object; }
namespace GUI
{
	class IMainGuiManager
	{
	public:
		virtual void setCaptionText(const char *text) = 0;
		virtual void addObject(const char *name, Engine::Scene::Object *obj) = 0;
	};
}
}
