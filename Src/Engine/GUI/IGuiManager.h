#pragma once

namespace Engine
{
namespace GUI
{
	class IGuiManager
	{
	public:
		virtual void center(int &x, int &y) = 0;
		virtual int getWidth() const = 0;
		virtual int getHeight() const = 0;
		virtual int getDesktopWidth() const = 0;
		virtual int getDesktopHeight() const = 0;
		virtual void swapBuffers() = 0;
	};
}
}
