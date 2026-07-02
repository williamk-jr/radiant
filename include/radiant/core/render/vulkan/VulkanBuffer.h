#pragma once

#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanMemoryAllocator.h"
#include <vulkan/vulkan_core.h>
namespace Radiant {
  /* Vulkan Buffer
   *
   * A wrapper for VkBuffer.
   * Represents a buffer resource in which various data is stored for rendering.
   */
  class VulkanBuffer {
    public:
      /*
       * @param VulkanMemoryAllocator& A reference to a valid vulkan memory allocator.
       * @param VkDeviceSize Buffer size in bytes.
       * @param VkBufferUsageFlagBits Flags to determine buffer usage.
       * @param VkSharingMode Mode determining how/if the buffer should be shared between queues of different families.
       * @param std::vector<uint32_t> Queue families to share buffer with if allowed by VkSharingMode.
       */
      VulkanBuffer(VulkanMemoryAllocator& memoryAllocator, VkDeviceSize size, VkBufferUsageFlagBits usage, VkSharingMode sharingMode, std::vector<uint32_t> queueFamilyIndicies);
      VulkanBuffer(const VulkanBuffer&) = delete;
      VulkanBuffer& operator=(const VulkanBuffer&) = delete;

      VulkanBuffer(VulkanBuffer&&) noexcept;
      VulkanBuffer& operator=(VulkanBuffer&&) noexcept = default;
      ~VulkanBuffer();

      /*
       * Copies provided data into the buffer.
       *
       * @param void* Pointer to data.
       * @param size_t Size of data.
       */
      void append(void* data, VkDeviceSize size);

      /*
       * @return Returns the current offset at which data will be appended.
       */
      VkDeviceSize getOffset();

      /*
       * @return Returns the current size in bytes of the buffer.
       */
      VkDeviceSize getSize();

      /*
       * Sets the data offset to zero.
       * Any subsequently appended data will begin to overwrite previous data.
       */
      void resetOffset();

      /*
       * @return A raw VkBuffer.
       */
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
