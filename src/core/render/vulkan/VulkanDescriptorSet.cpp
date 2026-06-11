#include "radiant/core/render/vulkan/VulkanDescriptorSet.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  VulkanDescriptorSet::VulkanDescriptorSet(VkDescriptorSet descriptorSet) : descriptorSet(descriptorSet) {
    
  }

  VulkanDescriptorSet::VulkanDescriptorSet(VulkanDescriptorSet&& other) noexcept :
    descriptorSet(other.descriptorSet) {
    other.descriptorSet = nullptr;
  }

  VulkanDescriptorSet::~VulkanDescriptorSet() {}

  VkDescriptorSet VulkanDescriptorSet::get() {
    return this->descriptorSet;
  }
}
