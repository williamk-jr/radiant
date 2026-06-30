#pragma once

#include "radiant/core/render/vulkan/VulkanSemaphore.h"
namespace Radiant {
  /* VulkanBinarySemaphore
   *
   * Wrapper for VkSemaphore with a binary semaphore type.
   */
  class VulkanBinarySemaphore : public VulkanSemaphore {
    public:
      /*
       * @param VulkanDevice& A reference to a valid vulkan device.
       * @param VkSemaphoreCreateFlags Create flags.
       */
      VulkanBinarySemaphore(VulkanDevice& device, VkSemaphoreCreateFlags flags);
      VulkanBinarySemaphore(const VulkanBinarySemaphore&) = delete;
      VulkanBinarySemaphore& operator=(const VulkanBinarySemaphore&) = delete;

      VulkanBinarySemaphore(VulkanBinarySemaphore&&) noexcept;
      VulkanBinarySemaphore& operator=(VulkanBinarySemaphore&&) noexcept = default;
  };
}
