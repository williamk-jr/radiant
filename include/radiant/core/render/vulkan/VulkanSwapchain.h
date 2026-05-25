#pragma once
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"
#include "radiant/core/render/vulkan/VulkanSurface.h"

#include <vulkan/vulkan_core.h>
namespace Radiant {
  class VulkanSwapchain {
    public:
      VulkanSwapchain(VulkanPhysicalDevice& physicalDevice, VulkanDevice& device, VulkanSurface& surface, VkImageUsageFlags imageUsageFlags, VkSwapchainCreateFlagsKHR swapchainFlags);
      ~VulkanSwapchain();
    private:
      VkSwapchainKHR swapchain;
      VulkanDevice& device;
      
      VkSurfaceFormat2KHR findSurfaceFormat(VulkanPhysicalDevice& physicalDevice, VulkanSurface& surface);
  };
}
