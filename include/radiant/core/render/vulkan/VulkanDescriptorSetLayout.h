#pragma once

#include "radiant/core/render/vulkan/VulkanDevice.h"
#include <vector>
#include <vulkan/vulkan_core.h>
namespace Radiant {

  class VulkanDescriptorSetLayout {
    public:
      VulkanDescriptorSetLayout(VulkanDevice& device, std::vector<VkDescriptorSetLayoutBinding> descriptorBindings);
      VulkanDescriptorSetLayout(const VulkanDescriptorSetLayout&) = delete;
      VulkanDescriptorSetLayout& operator=(const VulkanDescriptorSetLayout&) = delete;

      VulkanDescriptorSetLayout(VulkanDescriptorSetLayout&&) noexcept;
      VulkanDescriptorSetLayout& operator=(VulkanDescriptorSetLayout&&) noexcept = default;
      ~VulkanDescriptorSetLayout();

      VkDescriptorSetLayout get();
    private:
      VkDescriptorSetLayout layout;
      VkDevice device;
  };
}
