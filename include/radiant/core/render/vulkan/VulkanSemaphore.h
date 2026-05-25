#pragma once

#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include <vector>
#include <vulkan/vulkan_core.h>
namespace Radiant {
  class VulkanSemaphore {
    public:
      ~VulkanSemaphore();
      
      VkSemaphore get() const;

    protected:
      VulkanSemaphore(VulkanDevice& device, VkSemaphoreTypeCreateInfo type, VkSemaphoreCreateFlags flags);

      VulkanDevice& device;
    private:
      VkSemaphore semaphore;
  };
}
