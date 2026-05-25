#include "radiant/core/render/vulkan/VulkanSwapchain.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  VulkanSwapchain::VulkanSwapchain(VulkanPhysicalDevice& physicalDevice, VulkanDevice& device, VulkanSurface& surface, VkImageUsageFlags imageUsageFlags, VkSwapchainCreateFlagsKHR swapchainFlags) : device(device) {
    VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo{};
    surfaceInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
    surfaceInfo.surface = surface.get();

    VkSurfaceCapabilities2KHR surfaceCapabilities{};
    surfaceCapabilities.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;

    vkGetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice.get(), &surfaceInfo, &surfaceCapabilities);

    VkSurfaceFormat2KHR surfaceFormat = this->findSurfaceFormat(physicalDevice, surface);

    VkSwapchainCreateInfoKHR swapchainInfo{};
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.surface = surface.get();
    swapchainInfo.minImageCount = surfaceCapabilities.surfaceCapabilities.minImageCount;
    swapchainInfo.imageFormat = surfaceFormat.surfaceFormat.format;
    swapchainInfo.imageColorSpace = surfaceFormat.surfaceFormat.colorSpace;
    swapchainInfo.imageExtent = surfaceCapabilities.surfaceCapabilities.currentExtent;
    swapchainInfo.imageArrayLayers = 1;
    swapchainInfo.imageUsage = imageUsageFlags;
    swapchainInfo.flags = swapchainFlags;
    swapchainInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR; // Possibly add present mode selection;
    swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // Alpha selection;
    swapchainInfo.preTransform = surfaceCapabilities.surfaceCapabilities.currentTransform;
    swapchainInfo.clipped = VK_TRUE;

    vkCreateSwapchainKHR(device.get(), &swapchainInfo, nullptr, &this->swapchain);
  }
  VulkanSwapchain::~VulkanSwapchain() {
    vkDestroySwapchainKHR(this->device.get(), this->swapchain, nullptr); 
  }
  
  VkSurfaceFormat2KHR VulkanSwapchain::findSurfaceFormat(VulkanPhysicalDevice& physicalDevice, VulkanSurface& surface) {
    std::vector<VkSurfaceFormat2KHR> surfaceFormats = physicalDevice.getSurfaceFormats(surface);

    for (VkSurfaceFormat2KHR& format : surfaceFormats) {
      if (format.surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB && format.surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return format;
      }
    }

    return surfaceFormats[0];
  }
}
