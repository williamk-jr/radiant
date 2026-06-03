#pragma once
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanImage.h"
#include "radiant/core/render/vulkan/VulkanImageView.h"
#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"
#include "radiant/core/render/vulkan/VulkanSemaphore.h"
#include "radiant/core/render/vulkan/VulkanSurface.h"
#include "radiant/core/render/vulkan/VulkanResult.h"

#include <vulkan/vulkan_core.h>
namespace Radiant {
  class VulkanSwapchain {
    public:
      VulkanSwapchain(VulkanPhysicalDevice& physicalDevice, VulkanDevice& device, VulkanSurface& surface, VkImageUsageFlags imageUsageFlags, VkSwapchainCreateFlagsKHR swapchainFlags);
      VulkanSwapchain(const VulkanSwapchain&) = delete;
      VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;

      VulkanSwapchain(VulkanSwapchain&&) noexcept;
      VulkanSwapchain& operator=(VulkanSwapchain&&) noexcept = default;
      ~VulkanSwapchain();

      VkSwapchainKHR get();

      VkExtent2D getExtent(VulkanPhysicalDevice& physicalDevice, VulkanSurface& surface);
      VulkanResult<uint32_t> acquireNextImage(VulkanSemaphore* semaphore, uint64_t timeout);
      VulkanImage& getImage(uint32_t index);
      VulkanImageView& getImageView(uint32_t index);
      uint32_t getImageCount();

      void recreate(VulkanPhysicalDevice& physicalDevice, VulkanDevice& device, VulkanSurface& surface, VkImageUsageFlags imageUsageFlags, VkSwapchainCreateFlagsKHR swapchainFlags);
    private:
      VkSwapchainKHR swapchain;
      std::vector<VulkanImage> images;
      std::vector<VulkanImageView> imageViews;
      VkDevice device;
      
      void create(VulkanPhysicalDevice& physicalDevice, VulkanDevice& device, VulkanSurface& surface, VkImageUsageFlags imageUsageFlags, VkSwapchainCreateFlagsKHR swapchainFlags);
      void destroy();
      VkSurfaceFormat2KHR findSurfaceFormat(VulkanPhysicalDevice& physicalDevice, VulkanSurface& surface);
  };
}
