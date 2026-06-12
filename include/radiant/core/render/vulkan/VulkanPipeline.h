#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>
namespace Radiant {
  class VulkanPipeline {
    public:
      VulkanPipeline(VkDevice device, VkPipelineBindPoint bindPoint, VkPipeline pipeline, VkPipelineLayout layout, std::vector<VkShaderModule> shaderModules);
      VulkanPipeline(const VulkanPipeline&) = delete;
      VulkanPipeline& operator=(const VulkanPipeline&) = delete;

      VulkanPipeline(VulkanPipeline&&) noexcept;
      VulkanPipeline& operator=(VulkanPipeline&&) noexcept = default;
      ~VulkanPipeline();
      
      VkPipeline get();
      VkPipelineLayout getLayout();
      VkPipelineBindPoint getBindPoint();
    private:
      VkPipeline pipeline;
      VkPipelineBindPoint bindPoint;
      VkPipelineLayout layout;
      std::vector<VkShaderModule> shaderModules;
      VkDevice device;
  };
}
