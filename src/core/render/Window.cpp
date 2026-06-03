#include "radiant/core/render/Window.h"

namespace Radiant {
  Window::Window(const std::string& title, int width, int height) : title(title) {
#ifdef HAS_GLFW
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
#endif
  }
  
  Window::~Window() {
#ifdef HAS_GLFW
    glfwTerminate();
#endif
  }

  std::vector<const char*> Window::getSurfaceExtensions() {
#ifdef HAS_GLFW
    std::vector<const char*> extensionsVector;
    uint32_t extensionCount = 0;

    const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);
    for (int i = 0; i < extensionCount; i++) {
      extensionsVector.push_back(extensions[i]);
    }

    return extensionsVector;
#endif
  }
  
  Rect2D Window::getWindowSize() {
#ifdef HAS_GLFW
    int width = 0;
    int height = 0;

    glfwGetWindowSize(this->window, &width, &height);
    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
#endif
  }

  Rect2D Window::getFrameBufferSize() {
#ifdef HAS_GLFW
    int width = 0;
    int height = 0;

    glfwGetFramebufferSize(this->window, &width, &height);
    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
#endif
  }


  bool Window::shouldClose() {
#ifdef HAS_GLFW
    return glfwWindowShouldClose(this->window);
#endif
  }

  void Window::makeContextCurrent() {
#ifdef HAS_GLFW
    
#endif
  }

  void Window::pollEvents() {
#ifdef HAS_GLFW
    glfwPollEvents();
#endif
  }

  const std::string& Window::getTitle() {
    return this->title;
  }

#ifdef HAS_GLFW
  GLFWwindow* Window::getHandle() {
    return this->window;
  }
#endif
}
