#pragma once

#include <vulkan/vulkan_core.h>
namespace Radiant {
  /* VulkanDescriptorSet
   *
   * A wrapper for VkDescriptorSet.
   * Represents storage for a set of descriptors. 
   */
  class VulkanDescriptorSet {
    public:
      /*
       * @param VkDescriptorSet A raw descriptor set.
       */
      VulkanDescriptorSet(VkDescriptorSet descriptorSet);
      VulkanDescriptorSet(const VulkanDescriptorSet&) = delete;
      VulkanDescriptorSet& operator=(const VulkanDescriptorSet&) = delete;

      VulkanDescriptorSet(VulkanDescriptorSet&&) noexcept;
      VulkanDescriptorSet& operator=(VulkanDescriptorSet&&) noexcept = default;
      ~VulkanDescriptorSet();

      /*
       * @return A raw VkDescriptorSet.
       */
      VkDescriptorSet get();
    private:
      VkDescriptorSet descriptorSet;
  };
}
