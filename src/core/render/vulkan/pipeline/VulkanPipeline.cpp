#include "radiant/core/render/vulkan/pipeline/VulkanPipeline.h"
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Radiant {
  VulkanPipeline::VulkanPipeline(VkDevice device, VkPipelineBindPoint bindPoint, VkPipeline pipeline, VkPipelineLayout layout, VkPipelineCache cache, std::vector<VkShaderModule> shaderModules) : device(device), bindPoint(bindPoint), pipeline(pipeline), layout(layout), shaderModules(shaderModules) {
  }

  VulkanPipeline::VulkanPipeline(VulkanPipeline&& other) noexcept :
    pipeline(other.pipeline), bindPoint(other.bindPoint), layout(other.layout), shaderModules(other.shaderModules), device(other.device) {
    other.pipeline = nullptr;
    other.layout = nullptr;
    other.shaderModules.clear();
  }

  VulkanPipeline::~VulkanPipeline() {
    for (VkShaderModule& shaderModule : shaderModules) {
      vkDestroyShaderModule(this->device, shaderModule, nullptr);
    }
    vkDestroyPipelineLayout(this->device, this->layout, nullptr);
    vkDestroyPipeline(this->device, this->pipeline, nullptr);
  }

  VkPipeline VulkanPipeline::get() {
    return this->pipeline;
  }

  VkPipelineLayout VulkanPipeline::getLayout() {
    return this->layout;
  }

  VkPipelineBindPoint VulkanPipeline::getBindPoint() {
    return this->bindPoint;
  }
  
  VkPipelineCache VulkanPipeline::getCache() {
    return this->cache;
  }
}
