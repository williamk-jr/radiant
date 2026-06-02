#pragma once

#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include <vector>
#include <vulkan/vulkan_core.h>
namespace Radiant {
  class VulkanSemaphore {
    public:
      VulkanSemaphore(const VulkanSemaphore&) = delete;
      VulkanSemaphore& operator=(const VulkanSemaphore&) = delete;

      VulkanSemaphore(VulkanSemaphore&&) noexcept;
      VulkanSemaphore& operator=(VulkanSemaphore&&) noexcept = default;

      ~VulkanSemaphore();
      
      VkSemaphore get() const;

    protected:
      VulkanSemaphore(VulkanDevice& device, VkSemaphoreTypeCreateInfo type, VkSemaphoreCreateFlags flags);

      VkDevice device;
    private:
      VkSemaphore semaphore;
  };
}
