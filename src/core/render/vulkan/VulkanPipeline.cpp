#include "radiant/core/render/vulkan/VulkanPipeline.h"
#include "radiant/util/logger/Logger.h"
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Radiant {
  VulkanPipeline::VulkanPipeline(VkDevice device, VkPipeline pipeline, VkPipelineLayout layout, std::vector<VkShaderModule> shaderModules) : device(device), pipeline(pipeline), layout(layout) {
    this->shaderModules = shaderModules;
  }

  VulkanPipeline::VulkanPipeline(VulkanPipeline&& other) noexcept :
    pipeline(other.pipeline), layout(other.layout), shaderModules(other.shaderModules), device(other.device) {
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
}
