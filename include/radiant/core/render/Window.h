#pragma once
#include <string>
#include <vector>

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

#include "radiant/core/render/Rect2D.h"

namespace Radiant {
  class Window {
    public:
      Window(const std::string& title, int width, int height);
      ~Window();

      const std::string& getTitle();
      std::vector<const char*> getSurfaceExtensions();
      
      Rect2D getWindowSize(); 
      Rect2D getFrameBufferSize();

      bool shouldClose();
      void makeContextCurrent();
      void pollEvents();


#ifdef HAS_GLFW
      GLFWwindow* getHandle();
#endif

    private:
      const std::string& title;
#ifdef HAS_GLFW
      GLFWwindow* window;
#endif

  };
}
