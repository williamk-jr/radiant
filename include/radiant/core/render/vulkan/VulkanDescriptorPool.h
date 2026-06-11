#pragma once

#include "radiant/core/render/vulkan/VulkanDescriptorSet.h"
#include "radiant/core/render/vulkan/VulkanDescriptorSetLayout.h"
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include <vector>
#include <vulkan/vulkan_core.h>
namespace Radiant {
  class VulkanDescriptorPool {
    public:
      VulkanDescriptorPool(VulkanDevice& device, std::vector<VkDescriptorPoolSize> poolSizes, uint32_t maxDescriptorSets);
      VulkanDescriptorPool(const VulkanDescriptorPool&) = delete;
      VulkanDescriptorPool& operator=(const VulkanDescriptorPool&) = delete;

      VulkanDescriptorPool(VulkanDescriptorPool&&) noexcept;
      VulkanDescriptorPool& operator=(VulkanDescriptorPool&&) noexcept = default;
      ~VulkanDescriptorPool();

      VkDescriptorPool get();
      void reset();
      void allocateDescriptorSets(std::vector<VulkanDescriptorSetLayout>& descriptorSetLayouts);
      VulkanDescriptorSet& getDescriptorSet(uint32_t index);

    private:
      VkDescriptorPool descriptorPool;
      VkDevice device;

      std::vector<VulkanDescriptorSet> descriptorSets;
  };
}
