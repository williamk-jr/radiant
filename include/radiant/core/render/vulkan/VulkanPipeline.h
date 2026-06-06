#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>
namespace Radiant {
  class VulkanPipeline {
    public:
      VulkanPipeline(VkDevice device, VkPipeline pipeline, VkPipelineLayout layout, std::vector<VkShaderModule> shaderModules);
      VulkanPipeline(const VulkanPipeline&) = delete;
      VulkanPipeline& operator=(const VulkanPipeline&) = delete;

      VulkanPipeline(VulkanPipeline&&) noexcept;
      VulkanPipeline& operator=(VulkanPipeline&&) noexcept = default;
      ~VulkanPipeline();
      
      VkPipeline get();
    private:
      VkPipeline pipeline;
      VkPipelineLayout layout;
      std::vector<VkShaderModule> shaderModules;
      VkDevice device;
  };
}
