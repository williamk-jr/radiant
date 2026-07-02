#include "radiant/core/render/vulkan/VulkanSwapchain.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  VulkanSwapchain::VulkanSwapchain(VulkanPhysicalDevice& physicalDevice, VulkanDevice& device, VulkanSurface& surface, VkImageUsageFlags imageUsageFlags, VkPresentModeKHR presentMode, VkSwapchainCreateFlagsKHR swapchainFlags) {
    this->device = device.get();
    this->create(physicalDevice, device, surface, imageUsageFlags, presentMode, swapchainFlags);
  }

  VulkanSwapchain::VulkanSwapchain(VulkanSwapchain&& other) noexcept :
    swapchain(other.swapchain), device(other.device) {
    other.swapchain = nullptr;
  }

  VulkanSwapchain::~VulkanSwapchain() {
    this->destroy();
  }
  
  void VulkanSwapchain::create(VulkanPhysicalDevice& physicalDevice, VulkanDevice& device, VulkanSurface& surface, VkImageUsageFlags imageUsageFlags, VkPresentModeKHR presentMode, VkSwapchainCreateFlagsKHR swapchainFlags) {
    VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo{};
    surfaceInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
    surfaceInfo.surface = surface.get();

    VkSurfaceCapabilities2KHR surfaceCapabilities{};
    surfaceCapabilities.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;

    vkGetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice.get(), &surfaceInfo, &surfaceCapabilities);
    VkSurfaceFormat2KHR surfaceFormat = this->findSurfaceFormat(physicalDevice, surface);

    //VkExtent2D swapchainExtent{surfaceCapabilities.surfaceCapabilities.currentExtent};
    //if (swapchainExtent.width = 0xFFFFFFFF) {

    //}


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
    swapchainInfo.presentMode = presentMode; // Possibly add present mode selection;
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

    std::vector<VkImage> rawImages;
    rawImages.reserve(swapchainImageCount);
    this->images.reserve(swapchainImageCount);
    this->imageViews.reserve(swapchainImageCount);
    vkGetSwapchainImagesKHR(device.get(), this->swapchain, &swapchainImageCount, rawImages.data());

    VkImageSubresourceRange subresourceRange{};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;

    VkComponentMapping componentMapping{};
    componentMapping.r = VK_COMPONENT_SWIZZLE_R;
    componentMapping.g = VK_COMPONENT_SWIZZLE_G;
    componentMapping.b = VK_COMPONENT_SWIZZLE_B;
    componentMapping.a = VK_COMPONENT_SWIZZLE_A;

    for (int i = 0; i < swapchainImageCount; i++) {
      this->images.emplace_back(rawImages[i], surfaceCapabilities.surfaceCapabilities.currentExtent);
      this->imageViews.emplace_back(device, this->images[i], componentMapping, subresourceRange, 0);
    }
  }
  
  void VulkanSwapchain::destroy() {
    vkDestroySwapchainKHR(this->device, this->swapchain, nullptr); 
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

  VulkanImageView& VulkanSwapchain::getImageView(uint32_t index) {
    return this->imageViews[index];
  }

  uint32_t VulkanSwapchain::getImageCount() {
    return this->images.size();
  }

  VulkanResult<uint32_t> VulkanSwapchain::acquireNextImage(VulkanSemaphore* semaphore, uint64_t timeout) {
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(this->device, this->swapchain, timeout, semaphore->get(), nullptr, &imageIndex);
    //Validation::verify(result);
    return {result, imageIndex};
  }

  void VulkanSwapchain::recreate(VulkanPhysicalDevice& physicalDevice, VulkanDevice& device, VulkanSurface& surface, VkImageUsageFlags imageUsageFlags, VkPresentModeKHR presentMode, VkSwapchainCreateFlagsKHR swapchainFlags) {
    vkDeviceWaitIdle(this->device);

    this->imageViews.clear();
    this->images.clear();
    this->destroy(); 
    
    this->create(physicalDevice, device, surface, imageUsageFlags, presentMode, swapchainFlags);
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
