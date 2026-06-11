#pragma once

#include <vulkan/vulkan_core.h>
namespace Radiant {
  class VulkanDescriptorSet {
    public:
      VulkanDescriptorSet(VkDescriptorSet descriptorSet);
      VulkanDescriptorSet(const VulkanDescriptorSet&) = delete;
      VulkanDescriptorSet& operator=(const VulkanDescriptorSet&) = delete;

      VulkanDescriptorSet(VulkanDescriptorSet&&) noexcept;
      VulkanDescriptorSet& operator=(VulkanDescriptorSet&&) noexcept = default;
      ~VulkanDescriptorSet();

      VkDescriptorSet get();
    private:
      VkDescriptorSet descriptorSet;
  };
}
