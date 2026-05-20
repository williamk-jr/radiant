#pragma once
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  class VulkanCommandPool;

  class VulkanCommandBuffer {
    public:
      VulkanCommandBuffer(VkCommandBuffer commandBuffer);

    private:
      friend class VulkanCommandPool;
      VkCommandBuffer commandBuffer;
      
  };

  class VulkanCommandPool {
    public:
      VulkanCommandPool(VulkanDevice& device, uint32_t queueFamily);
      ~VulkanCommandPool();
      
      void reset(bool recycleResources);
      std::vector<VulkanCommandBuffer> allocateCommandBuffers(int count, VkCommandBufferLevel level);
    private:
      VkCommandPool commandPool;
      VulkanDevice& device;
  };
}
