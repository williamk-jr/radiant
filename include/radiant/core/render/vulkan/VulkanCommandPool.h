#pragma once
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanImage.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include "radiant/core/render/vulkan/VulkanCommandBuffer.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  /* VulkanCommandPool
   * 
   * Wrapper for VkCommandPool.
   * A pool of resources from which all command buffers are allocated from.
   */
  class VulkanCommandPool {
    public:
      /*
       * @param VulkanDevice& A reference to a valid vulkan device.
       * @param uint32_t Index of queue family used to create command pool. 
       */
      VulkanCommandPool(VulkanDevice& device, uint32_t queueFamily);
      VulkanCommandPool(const VulkanCommandPool&) = delete;
      VulkanCommandPool& operator=(const VulkanCommandPool&) = delete;

      VulkanCommandPool(VulkanCommandPool&&) noexcept;
      VulkanCommandPool& operator=(VulkanCommandPool&&) noexcept = default;
      ~VulkanCommandPool();
      
      /*
       * Resets command pool, freeing all reserved resources.
       *
       * @param bool Whether to recycle resources.
       */
      void reset(bool recycleResources);

      /*
       * Allocates command buffers.
       *
       * @param int Number of command buffers to allocate.
       * @param VkCommandBufferLevel The level allocated command buffers will be. 
       */
      std::vector<VulkanCommandBuffer> allocateCommandBuffers(int count, VkCommandBufferLevel level);

      /*
       * Allocates a single command buffer.
       *
       * @param VkCommandBufferLevel The level allocated command buffers will be. 
       */
      VulkanCommandBuffer allocateCommandBuffer(VkCommandBufferLevel level);

      /*
       * Frees allocated command buffers.
       * 
       * @param std::vector<VulkanCommandBuffer> command buffers to free.
       */
      void freeCommandBuffers(std::vector<VulkanCommandBuffer> commandBuffers);
    private:
      VkCommandPool commandPool;
      VkDevice device;
  };
}
