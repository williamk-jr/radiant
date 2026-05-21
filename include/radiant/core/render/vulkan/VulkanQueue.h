#pragma once
#include <cstdint>
#include "radiant/core/render/vulkan/VulkanCommandPool.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  class VulkanQueue {
    public:
      VulkanQueue(VulkanDevice& device, uint32_t queueFamily, uint32_t queueIndex); 

      void submit(std::vector<VulkanCommandBuffer>& commandBuffer);
    private:
      VkQueue queue;
  };
}
