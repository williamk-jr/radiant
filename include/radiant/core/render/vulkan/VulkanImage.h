#pragma once
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanMemoryAllocator.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  class VulkanImage {
    public:
      VulkanImage(VulkanMemoryAllocator& allocator, VkExtent3D extent);
      VulkanImage(VkImage image);
      VulkanImage(const VulkanImage&) = delete;
      VulkanImage& operator=(const VulkanImage&) = delete;

      VulkanImage(VulkanImage&&) noexcept;
      VulkanImage& operator=(VulkanImage&&) noexcept = default;
      ~VulkanImage();
      
      VkImage get();
      VkImageMemoryBarrier2 createMemoryBarrier();

    private:
      VkImage image;
      VmaAllocation imageMemory;
      VulkanMemoryAllocator* allocator;
  };
}
