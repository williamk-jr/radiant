#pragma once

#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanMemoryAllocator.h"
#include <vulkan/vulkan_core.h>
namespace Radiant {
  class VulkanBuffer {
    public:
      VulkanBuffer(VulkanMemoryAllocator& memoryAllocator, VkDeviceSize size, VkBufferUsageFlagBits usage, VkSharingMode sharingMode, std::vector<uint32_t> queueFamilyIndicies);
      VulkanBuffer(const VulkanBuffer&) = delete;
      VulkanBuffer& operator=(const VulkanBuffer&) = delete;

      VulkanBuffer(VulkanBuffer&&) noexcept;
      VulkanBuffer& operator=(VulkanBuffer&&) noexcept = default;
      ~VulkanBuffer();

      void append(void* data, size_t size);
      size_t getOffset();
      void resetOffset();

      VkBuffer get();
    private:
      VkBuffer buffer;
      VmaAllocation allocation;
      VmaAllocationInfo allocationInfo{};
      VmaAllocator memoryAllocator;

      VkDeviceSize size = 0;
      VkDeviceSize offset = 0;
  };
}
