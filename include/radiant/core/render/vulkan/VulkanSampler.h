#pragma once
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanMemoryAllocator.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  class VulkanSampler {
    public:
      VulkanSampler(VulkanDevice& device, VkSamplerCreateFlags flags);

      VulkanSampler(const VulkanSampler&) = delete;
      VulkanSampler& operator=(const VulkanSampler&) = delete;

      VulkanSampler(VulkanSampler&&) noexcept;
      VulkanSampler& operator=(VulkanSampler&&) noexcept = default;
      ~VulkanSampler();
      
      VkSampler get();

    private:
      VkSampler sampler;
      VkDevice device;
  };
}
