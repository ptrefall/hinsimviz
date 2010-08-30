#pragma once

namespace GUI { class IGuiManager; }

namespace Core
{
	class CoreManager
	{
	public:
		CoreManager(GUI::IGuiManager *guiManager);
		~CoreManager();

		void frame();

	private:
		GUI::IGuiManager *guiManager;
	};
}
