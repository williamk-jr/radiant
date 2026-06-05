#include "radiant/core/render/vulkan/VulkanPipeline.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  VulkanPipeline::VulkanPipeline(VkDevice device, VkPipeline pipeline) : device(device), pipeline(pipeline) {
  }
  
  VulkanPipeline::~VulkanPipeline() {
    vkDestroyPipeline(this->device, this->pipeline, nullptr);
  }

  VkPipeline VulkanPipeline::get() {
    return this->pipeline;
  }
}
