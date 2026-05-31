#pragma once
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanImage.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include "radiant/core/render/vulkan/VulkanCommandBuffer.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  class VulkanCommandPool {
    public:
      VulkanCommandPool(VulkanDevice& device, uint32_t queueFamily);
      VulkanCommandPool(const VulkanCommandPool&) = delete;
      VulkanCommandPool& operator=(const VulkanCommandPool&) = delete;

      VulkanCommandPool(VulkanCommandPool&&) noexcept;
      VulkanCommandPool& operator=(VulkanCommandPool&&) noexcept = default;
      ~VulkanCommandPool();
      
      void reset(bool recycleResources);
      std::vector<VulkanCommandBuffer> allocateCommandBuffers(int count, VkCommandBufferLevel level);
      void freeCommandBuffers(std::vector<VulkanCommandBuffer> commandBuffers);
    private:
      VkCommandPool commandPool;
      VulkanDevice& device;
  };
}
