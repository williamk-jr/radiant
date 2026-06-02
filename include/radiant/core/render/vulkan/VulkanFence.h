#pragma once

#include "radiant/core/render/vulkan/VulkanDevice.h"
#include <vulkan/vulkan_core.h>
namespace Radiant {
  class VulkanFence {
    public:
      VulkanFence(VulkanDevice& device, VkFenceCreateFlags flags);
      VulkanFence(const VulkanFence&) = delete;
      VulkanFence& operator=(const VulkanFence&) = delete;

      VulkanFence(VulkanFence&&) noexcept;
      VulkanFence& operator=(VulkanFence&&) noexcept = default;
      ~VulkanFence();

      VkFence get();
      void reset();
      void wait(uint32_t timeout);
      bool isSignaled();
      static void waitForFences(VulkanDevice& device, std::vector<VulkanFence>& fences, bool waitAll, uint64_t timeout);
    private:
      VkFence fence;
      VkDevice device;
  };
}
