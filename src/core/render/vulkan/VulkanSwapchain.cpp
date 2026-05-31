#include "radiant/core/render/vulkan/VulkanSwapchain.h"
#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"
#include "radiant/core/render/vulkan/VulkanSemaphore.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include <cstdint>
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

    uint32_t queueFamilies[] = {device.getGraphicsQueueFamily(), device.getPresentQueueFamily()};
    if (device.getGraphicsQueueFamily() != device.getPresentQueueFamily()) {
      swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      swapchainInfo.queueFamilyIndexCount = 2;
      swapchainInfo.pQueueFamilyIndices = queueFamilies;
    } else {
      swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      swapchainInfo.queueFamilyIndexCount = 0;
      swapchainInfo.pQueueFamilyIndices = nullptr;
    }

    Validation::verify(
      vkCreateSwapchainKHR(device.get(), &swapchainInfo, nullptr, &this->swapchain)
    );
    
    uint32_t swapchainImageCount = 0;
    vkGetSwapchainImagesKHR(device.get(), this->swapchain, &swapchainImageCount, nullptr);
    std::vector<VkImage> rawImages(swapchainImageCount);
    this->images.reserve(swapchainImageCount);
    vkGetSwapchainImagesKHR(device.get(), this->swapchain, &swapchainImageCount, rawImages.data());

    for (VkImage rawImage : rawImages) {
      this->images.emplace_back(rawImage);
    }
  }

  VulkanSwapchain::VulkanSwapchain(VulkanSwapchain&& other) noexcept :
    swapchain(other.swapchain), device(other.device) {
    other.swapchain = nullptr;
  }

  VulkanSwapchain::~VulkanSwapchain() {
    vkDestroySwapchainKHR(this->device.get(), this->swapchain, nullptr); 
  }
  
  VkExtent2D VulkanSwapchain::getExtent(VulkanPhysicalDevice& physicalDevice, VulkanSurface& surface) {
    VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo{};
    surfaceInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
    surfaceInfo.surface = surface.get();

    VkSurfaceCapabilities2KHR surfaceCapabilities{};
    surfaceCapabilities.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;

    vkGetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice.get(), &surfaceInfo, &surfaceCapabilities);
    return surfaceCapabilities.surfaceCapabilities.currentExtent;
  }


  VkSwapchainKHR VulkanSwapchain::get() {
    return this->swapchain;
  }

  VulkanImage& VulkanSwapchain::getImage(uint32_t index) {
    return this->images[index];
  }

  uint32_t VulkanSwapchain::getImageCount() {
    return this->images.size();
  }

  uint32_t VulkanSwapchain::acquireNextImage(VulkanSemaphore* semaphore, uint64_t timeout) {
    uint32_t imageIndex;
    vkAcquireNextImageKHR(this->device.get(), this->swapchain, timeout, semaphore->get(), nullptr, &imageIndex);
    return imageIndex;
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
