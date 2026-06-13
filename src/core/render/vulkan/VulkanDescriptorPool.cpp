#include "radiant/core/render/vulkan/VulkanDescriptorPool.h"
#include "radiant/core/render/vulkan/VulkanDescriptorSet.h"
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include <algorithm>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Radiant {
  VulkanDescriptorPool::VulkanDescriptorPool(VulkanDevice& device, std::vector<VkDescriptorPoolSize> poolSizes, uint32_t maxDescriptorSets) : 
    device(device.get()) {

    VkDescriptorPoolCreateInfo descriptorPoolInfo{};
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.poolSizeCount = poolSizes.size();
    descriptorPoolInfo.pPoolSizes = poolSizes.data();
    descriptorPoolInfo.maxSets = maxDescriptorSets;
    descriptorPoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;

    vkCreateDescriptorPool(device.get(), &descriptorPoolInfo, nullptr, &this->descriptorPool);
  }
  
  VulkanDescriptorPool::VulkanDescriptorPool(VulkanDescriptorPool&& other) noexcept :
    descriptorPool(other.descriptorPool), device(other.device) {
    other.descriptorPool = nullptr;
  }

  VulkanDescriptorPool::~VulkanDescriptorPool() {
    vkDestroyDescriptorPool(this->device, this->descriptorPool, nullptr);
  }

  VkDescriptorPool VulkanDescriptorPool::get() {
    return this->descriptorPool;
  }
  
  void VulkanDescriptorPool::reset() {
    vkResetDescriptorPool(this->device, this->descriptorPool, 0);
  }

  std::vector<VulkanDescriptorSet> VulkanDescriptorPool::allocateDescriptorSets(std::vector<VulkanDescriptorSetLayout>& descriptorSetLayouts) {
    std::vector<VkDescriptorSetLayout> rawDescriptorSetLayouts;
    rawDescriptorSetLayouts.reserve(descriptorSetLayouts.size());

    for (VulkanDescriptorSetLayout& descriptorSetLayout : descriptorSetLayouts) {
      rawDescriptorSetLayouts.emplace_back(descriptorSetLayout.get());
    }

    std::vector<VkDescriptorSet> rawDescriptorSets(descriptorSetLayouts.size());
    //rawDescriptorSets.reserve(descriptorSetLayouts.size());

    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{};
    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateInfo.descriptorPool = this->descriptorPool;
    descriptorSetAllocateInfo.descriptorSetCount = rawDescriptorSetLayouts.size();
    descriptorSetAllocateInfo.pSetLayouts = rawDescriptorSetLayouts.data();

    vkAllocateDescriptorSets(this->device, &descriptorSetAllocateInfo, rawDescriptorSets.data());

    std::vector<VulkanDescriptorSet> wrappedDescriptorSets;
    wrappedDescriptorSets.reserve(descriptorSetLayouts.size());

    for (VkDescriptorSet& descriptorSet : rawDescriptorSets) {
      wrappedDescriptorSets.emplace_back(descriptorSet);
    }
    //Logger::info(std::to_string(rawDescriptorSets.size()));
    return wrappedDescriptorSets;
  }
  
  void VulkanDescriptorPool::updateDescriptorSets(std::vector<VulkanWriteDescriptorSet> descriptorSetWrites, std::vector<VulkanCopyDescriptorSet> descriptorSetCopies) {
    std::vector<VkWriteDescriptorSet> rawDescriptorWrites;
    rawDescriptorWrites.reserve(descriptorSetWrites.size());

    for (VulkanWriteDescriptorSet& descriptorWrite : descriptorSetWrites) {
      uint32_t descriptorCount = std::max({
          descriptorWrite.imageInfo.size(), 
          descriptorWrite.bufferInfo.size(), 
          descriptorWrite.texelBufferViews.size()
      });

      rawDescriptorWrites.emplace_back(VkWriteDescriptorSet{
        VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, 
        descriptorWrite.descriptorSet,
        descriptorWrite.descriptorBindingIndex, 
        descriptorWrite.descriptorArrayElement,
        descriptorCount,
        descriptorWrite.descriptorType, 
        descriptorWrite.imageInfo.size() != 0 ? descriptorWrite.imageInfo.data() : nullptr, 
        descriptorWrite.bufferInfo.size() != 0 ? descriptorWrite.bufferInfo.data() : nullptr, 
        descriptorWrite.texelBufferViews.size() != 0 ? descriptorWrite.texelBufferViews.data() : nullptr
      });
    }

    std::vector<VkCopyDescriptorSet> rawDescriptorCopies;
    rawDescriptorCopies.reserve(descriptorSetCopies.size());

    for (VulkanCopyDescriptorSet& descriptorCopy : descriptorSetCopies) {
      rawDescriptorCopies.emplace_back(VkCopyDescriptorSet{
        VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET, nullptr, 
        descriptorCopy.srcDescriptorSet,
        descriptorCopy.srcDecriptorBindingIndex,
        descriptorCopy.srcDescriptorArrayElement,
        descriptorCopy.dstDescriptorSet,
        descriptorCopy.dstDecriptorBindingIndex,
        descriptorCopy.dstDescriptorArrayElement,
        descriptorCopy.descriptorCount
      });
    }

    vkUpdateDescriptorSets(this->device, 
        rawDescriptorWrites.size(), descriptorSetWrites.size() != 0 ? rawDescriptorWrites.data() : nullptr, 
        rawDescriptorCopies.size(), descriptorSetCopies.size() != 0 ? rawDescriptorCopies.data() : nullptr
    );
  }
}
