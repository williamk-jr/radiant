#include "radiant/core/render/WindowSystem.h"

#include <GLFW/glfw3.h>
#include <memory>

namespace Radiant {
	WindowSystem::WindowSystem() {
#ifdef HAS_GLFW
		glfwInit();
#endif
	}

	WindowSystem::~WindowSystem() {
#ifdef HAS_GLFW
		glfwTerminate();
#endif
	}

	std::unique_ptr<Window> WindowSystem::createWindow(const std::string& title, int width, int height) {
		return std::make_unique<Window>(Window{title, width, height});
	}
} // namespace Radiant
