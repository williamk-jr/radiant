#pragma once

#include <vulkan/vulkan_core.h>
namespace Radiant {
  class VulkanPipeline {
    public:
      VulkanPipeline(VkDevice device, VkPipeline pipeline);
      ~VulkanPipeline();
      VkPipeline get();
    private:
      VkPipeline pipeline;
      VkDevice device;
  };
}
