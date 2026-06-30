#pragma once

#include "radiant/core/render/vulkan/VulkanDevice.h"
#include <vector>
#include <vulkan/vulkan_core.h>
namespace Radiant {

  /* VulkanDescriptorSetLayout
   *
   * Wrapper for VkDescriptorSetLayout.
   * Represents the layout of descriptors in a descriptor set.
   */
  class VulkanDescriptorSetLayout {
    public:
      /*
       * @param VulkanDevice& A reference to a valid vulkan device.
       * @param std::vector<VkDescriptorSetLayoutBinding> A vector of layout bindings for each descriptor in the descriptor set.
       */
      VulkanDescriptorSetLayout(VulkanDevice& device, std::vector<VkDescriptorSetLayoutBinding> descriptorBindings);
      VulkanDescriptorSetLayout(const VulkanDescriptorSetLayout&) = delete;
      VulkanDescriptorSetLayout& operator=(const VulkanDescriptorSetLayout&) = delete;

      VulkanDescriptorSetLayout(VulkanDescriptorSetLayout&&) noexcept;
      VulkanDescriptorSetLayout& operator=(VulkanDescriptorSetLayout&&) noexcept = default;
      ~VulkanDescriptorSetLayout();

      /*
       * @return A raw VkDescriptorSetLayout.
       */
      VkDescriptorSetLayout get();
    private:
      VkDescriptorSetLayout layout;
      VkDevice device;
  };
}
