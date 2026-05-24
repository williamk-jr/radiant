#include "radiant/core/render/vulkan/VulkanSurface.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include <stdexcept>
#include <string>
#include <vulkan/vulkan_core.h>

namespace Radiant {
#ifdef HAS_GLFW
  VulkanSurface::VulkanSurface(VulkanInstance& instance, GLFWwindow* window) : instance(instance) {
    Validation::verify(
      glfwCreateWindowSurface(instance.get(), window, nullptr, &this->surface)
    );
  }
#endif 

#ifdef HAS_SDL3
    SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface)
#endif
  VulkanSurface::~VulkanSurface() {
    vkDestroySurfaceKHR(this->instance.get(), this->surface, nullptr);
  }
  
  VkSurfaceKHR VulkanSurface::get() {
    return this->surface;
  }
}
