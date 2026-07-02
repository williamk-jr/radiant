#pragma once
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanMemoryAllocator.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  class VulkanImage {
    public:
      VulkanImage(VulkanMemoryAllocator& allocator, VkExtent3D extent);
      VulkanImage(VkImage image, VkExtent2D extent);
      VulkanImage(const VulkanImage&) = delete;
      VulkanImage& operator=(const VulkanImage&) = delete;

      VulkanImage(VulkanImage&&) noexcept;
      VulkanImage& operator=(VulkanImage&&) noexcept = default;
      ~VulkanImage();
      
      VkImage get();
      VkExtent3D getExtent();
      VkImageMemoryBarrier2 createMemoryBarrier();

    private:
      VkImage image;
      VkExtent3D extent;
      VmaAllocation imageMemory;
      VmaAllocator memoryAllocator;
      //VulkanMemoryAllocator* allocator;
  };
}
