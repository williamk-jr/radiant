#pragma once

#include "radiant/core/render/vulkan/VulkanDescriptorSet.h"
#include "radiant/core/render/vulkan/VulkanDescriptorSetLayout.h"
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include <vector>
#include <vulkan/vulkan_core.h>
namespace Radiant {
  struct VulkanWriteDescriptorSet {
    VkDescriptorSet descriptorSet;
    uint32_t descriptorBindingIndex;
    uint32_t descriptorArrayElement;
    VkDescriptorType descriptorType;

    std::vector<VkDescriptorBufferInfo> bufferInfo;
    std::vector<VkDescriptorImageInfo> imageInfo;
    std::vector<VkBufferView> texelBufferViews;
  };

  struct VulkanCopyDescriptorSet {
    VkDescriptorSet srcDescriptorSet;
    uint32_t srcDecriptorBindingIndex;
    uint32_t srcDescriptorArrayElement;

    VkDescriptorSet dstDescriptorSet;
    uint32_t dstDecriptorBindingIndex;
    uint32_t dstDescriptorArrayElement;
    uint32_t descriptorCount;
  };

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

      std::vector<VulkanDescriptorSet> allocateDescriptorSets(std::vector<VulkanDescriptorSetLayout>& descriptorSetLayouts);
      void updateDescriptorSets(std::vector<VulkanWriteDescriptorSet> descriptorSetWrites, std::vector<VulkanCopyDescriptorSet> descriptorSetCopies);
      VulkanDescriptorSet& getDescriptorSet(uint32_t index);

    private:
      VkDescriptorPool descriptorPool;
      VkDevice device;
  };
}
