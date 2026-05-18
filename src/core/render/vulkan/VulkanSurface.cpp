#include "radiant/core/render/vulkan/VulkanSurface.h"
#include <stdexcept>
#include <string>
#include <vulkan/vulkan_core.h>

namespace Radiant {
#ifdef HAS_GLFW
  VulkanSurface::VulkanSurface(VulkanInstance& instance, GLFWwindow* window) : instance(instance) {
    // TODO implement surface.
    VkResult result = glfwCreateWindowSurface(instance.get(), window, nullptr, &this->surface); 
    if (result != VK_SUCCESS) {
      throw std::runtime_error("Failed to create surface: " + std::to_string(result));
    }
  }
#endif 

#ifdef HAS_SDL3
    SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface)
#endif
    VulkanSurface::~VulkanSurface() {
      vkDestroySurfaceKHR(this->instance.get(), this->surface, nullptr);
    }
  
  VkSurfaceKHR& VulkanSurface::get() {
    return this->surface;
  }
}
