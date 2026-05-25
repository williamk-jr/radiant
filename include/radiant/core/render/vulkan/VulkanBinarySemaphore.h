#pragma once

#include "radiant/core/render/vulkan/VulkanSemaphore.h"
namespace Radiant {
  class VulkanBinarySemaphore : public VulkanSemaphore {
    public: 
      VulkanBinarySemaphore(VulkanDevice& device, VkSemaphoreCreateFlags flags);

  };
}
