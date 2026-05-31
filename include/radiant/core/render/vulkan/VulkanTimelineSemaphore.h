#pragma once

#include "radiant/core/render/vulkan/VulkanSemaphore.h"
namespace Radiant {
  class VulkanTimelineSemaphore : public VulkanSemaphore {
    public: 
      VulkanTimelineSemaphore(VulkanDevice& device, VkSemaphoreCreateFlags flags, uint64_t initialValue);
      VulkanTimelineSemaphore(const VulkanTimelineSemaphore&) = delete;
      VulkanTimelineSemaphore& operator=(const VulkanTimelineSemaphore&) = delete;

      VulkanTimelineSemaphore(VulkanTimelineSemaphore&&) noexcept;
      VulkanTimelineSemaphore& operator=(VulkanTimelineSemaphore&&) noexcept = default;

      uint64_t getCounterValue();
      void signal(uint64_t value);

      static void waitSemaphores(VulkanDevice& device, std::vector<VulkanTimelineSemaphore>& semaphores, std::vector<uint64_t>& values, VkSemaphoreWaitFlags flags, uint64_t timeout);

  };
}
