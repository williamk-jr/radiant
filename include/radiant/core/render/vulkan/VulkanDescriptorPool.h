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

  /* VulkanDescriptorPool
   *
   * Wrapper for VkDescriptorPool.
   * A pool of resources rom which all descriptor sets are allocated from.
   */
  class VulkanDescriptorPool {
    public:
      /*
       * @param VulkanDevice& A reference to a valid vulkan device.
       * @param std::vector<VkDescriptorPoolSize> A vector of sizes for each descriptor pool.
       * @param uint32_t The maximum number of descriptor sets that can be allocated.
       */
      VulkanDescriptorPool(VulkanDevice& device, std::vector<VkDescriptorPoolSize> poolSizes, uint32_t maxDescriptorSets);
      VulkanDescriptorPool(const VulkanDescriptorPool&) = delete;
      VulkanDescriptorPool& operator=(const VulkanDescriptorPool&) = delete;

      VulkanDescriptorPool(VulkanDescriptorPool&&) noexcept;
      VulkanDescriptorPool& operator=(VulkanDescriptorPool&&) noexcept = default;
      ~VulkanDescriptorPool();

      /*
       * @return A raw VkDescriptorPool.
       */
      VkDescriptorPool get();

      /*
       * Resets descriptor pool, freeing all allocated descriptor sets.
       */
      void reset();

      /*
       * Allocates descriptor sets.
       *
       * @param std::vector<VulkanDescriptorSetLayout>& A reference to a vector o descriptor set layouts.
       * @return A vector of descriptor sets.
       */
      std::vector<VulkanDescriptorSet> allocateDescriptorSets(std::vector<VulkanDescriptorSetLayout>& descriptorSetLayouts);

      /*
       * Updates descriptor sets.
       *
       * @param std::vector<VulkanWriteDescriptorSet>> A vector of write instructions for each descriptor to be updated.
       * @param std::vector<VulkanCopyDescriptorSet>> A vector of copy instructions for each descriptor to be updated.
       */
      void updateDescriptorSets(std::vector<VulkanWriteDescriptorSet> descriptorSetWrites, std::vector<VulkanCopyDescriptorSet> descriptorSetCopies);

      /*
       * Updates descriptor sets.
       *
       * @param std::vector<VulkanWriteDescriptorSet>> A vector of write instructions for each descriptor to be updated.
       */
      void updateDescriptorSets(std::vector<VulkanWriteDescriptorSet> descriptorSetWrites);
      
      /*
       * Updates descriptor sets.
       *
       * @param std::vector<VulkanCopyDescriptorSet>> A vector of copy instructions for each descriptor to be updated.
       */
      void updateDescriptorSets(std::vector<VulkanCopyDescriptorSet> descriptorSetCopies);



    private:
      VkDescriptorPool descriptorPool;
      VkDevice device;

      std::vector<VkWriteDescriptorSet> toRawDescriptorWrites(std::vector<VulkanWriteDescriptorSet>& descriptorSetWrites);
      std::vector<VkCopyDescriptorSet> toRawDescriptorCopies(std::vector<VulkanCopyDescriptorSet>& descriptorSetWrites);
  };
}
