#pragma once

namespace Engine
{
namespace GUI
{
	class IMainGuiManager
	{
	public:
		virtual void setCaptionText(const char *text) = 0;
		virtual void addObject(const char *name) = 0;
	};
}
}
