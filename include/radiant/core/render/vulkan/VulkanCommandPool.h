#pragma once

#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include <vulkan/vulkan_core.h>
namespace Radiant {
  class VulkanCommandPool {
    public:
      VulkanCommandPool(VulkanDevice& device, uint32_t queueFamily);
      ~VulkanCommandPool();
    private:
      VkCommandPool commandPool;
      VulkanDevice& device;
  };
}
