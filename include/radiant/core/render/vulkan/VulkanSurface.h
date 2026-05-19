#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#ifdef HAS_GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif

#ifdef HAS_SDL3
#include <SDL3/SDL.h>
#endif

#include "radiant/core/render/vulkan/VulkanInstance.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"

namespace Radiant {
  class VulkanSurface {
    public:
#ifdef HAS_GLFW
      VulkanSurface(VulkanInstance& instance, GLFWwindow* window);
#endif
      ~VulkanSurface();

      VkSurfaceKHR& get();
    private:
      VkSurfaceKHR surface;
      VulkanInstance& instance;
  };
}
