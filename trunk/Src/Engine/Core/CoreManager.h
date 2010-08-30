#pragma once

namespace GUI { class IGuiManager; }

namespace Core
{
	class CoreManager
	{
	public:
		CoreManager(GUI::IGuiManager *guiManager);

	private:
		GUI::IGuiManager *guiManager;
	};
}
