#pragma once

#include "radiant/core/render/vulkan/VulkanSemaphore.h"
namespace Radiant {
  class VulkanBinarySemaphore : public VulkanSemaphore {
    public: 
      VulkanBinarySemaphore(VulkanDevice& device, VkSemaphoreCreateFlags flags);
      VulkanBinarySemaphore(const VulkanBinarySemaphore&) = delete;
      VulkanBinarySemaphore& operator=(const VulkanBinarySemaphore&) = delete;

      VulkanBinarySemaphore(VulkanBinarySemaphore&&) noexcept;
      VulkanBinarySemaphore& operator=(VulkanBinarySemaphore&&) noexcept = default;

  };
}
