#pragma once

#include "radiant/core/render/vulkan/VulkanDevice.h"
#include <vulkan/vulkan_core.h>
namespace Radiant {
  class VulkanFence {
    public:
      VulkanFence(VulkanDevice& device, VkFenceCreateFlags flags);
      ~VulkanFence();

      VkFence get();
      bool isSignaled();
      static void waitForFences(VulkanDevice& device, std::vector<VulkanFence>& fences, bool waitAll, uint64_t timeout);
    private:
      VkFence fence;
      VulkanDevice& device;
  };
}
