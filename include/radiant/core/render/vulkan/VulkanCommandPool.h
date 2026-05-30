#pragma once
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanImage.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  class VulkanCommandPool;

  class VulkanCommandBuffer {
    public:
      VulkanCommandBuffer(VkCommandBuffer commandBuffer);
      VkCommandBuffer get();

      void begin(VkCommandBufferUsageFlags flags);
      void pipelineMemoryBarrier(std::vector<VkMemoryBarrier2>& memoryBarriers, VkDependencyFlags dependencyFlags);
      void pipelineImageMemoryBarrier(std::vector<VkImageMemoryBarrier2>& memoryBarriers, VkDependencyFlags dependencyFlags);
      void pipelineBufferMemoryBarrier(std::vector<VkBufferMemoryBarrier2>& memoryBarriers, VkDependencyFlags dependencyFlags);
      void clearColor(VulkanImage& image, VkClearColorValue& color);
      //void beginRendering();


      void end();
      void reset(bool recycleResources);

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
      void freeCommandBuffers(std::vector<VulkanCommandBuffer> commandBuffers);
    private:
      VkCommandPool commandPool;
      VulkanDevice& device;
  };
}
