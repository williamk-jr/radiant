#include "radiant/core/render/vulkan/VulkanSurface.h"

namespace Radiant {
#ifdef HAS_GLFW
  VulkanSurface::VulkanSurface(VulkanInstance& instance, GLFWwindow* window) : instance(instance.get()) {
    Validation::verify(
      glfwCreateWindowSurface(instance.get(), window, nullptr, &this->surface)
    );
  }
#endif 

#ifdef HAS_SDL3
    SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface)
#endif

  VulkanSurface::VulkanSurface(VulkanSurface&& other) noexcept :
    surface(other.surface), instance(other.instance) {
    other.surface = nullptr;
  }

  VulkanSurface::~VulkanSurface() {
    vkDestroySurfaceKHR(this->instance, this->surface, nullptr);
  }
  
  VkSurfaceKHR VulkanSurface::get() {
    return this->surface;
  }
}
