#include "radiant/core/render/vulkan/VulkanDescriptorPool.h"
#include "radiant/core/render/vulkan/VulkanDescriptorSet.h"
#include "radiant/core/render/vulkan/VulkanDevice.h"
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
    descriptorPoolInfo.flags = 0;

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

  void VulkanDescriptorPool::allocateDescriptorSets(std::vector<VulkanDescriptorSetLayout>& descriptorSetLayouts) {
    std::vector<VkDescriptorSetLayout> rawDescriptorSetLayouts;
    rawDescriptorSetLayouts.reserve(descriptorSetLayouts.size());
    for (VulkanDescriptorSetLayout& descriptorSetLayout : descriptorSetLayouts) {
      rawDescriptorSetLayouts.emplace_back(descriptorSetLayout.get());
    }

    std::vector<VkDescriptorSet> rawDescriptorSets;
    rawDescriptorSets.reserve(descriptorSetLayouts.size());

    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{};
    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateInfo.descriptorPool = this->descriptorPool;
    descriptorSetAllocateInfo.descriptorSetCount = rawDescriptorSetLayouts.size();
    descriptorSetAllocateInfo.pSetLayouts = rawDescriptorSetLayouts.data();

    vkAllocateDescriptorSets(this->device, &descriptorSetAllocateInfo, rawDescriptorSets.data());

    std::vector<VulkanDescriptorSet> wrappedDescriptorSets;
    this->descriptorSets.reserve(rawDescriptorSets.size());
    for (VkDescriptorSet& descriptorSet : rawDescriptorSets) {
      this->descriptorSets.emplace_back(descriptorSet);
    }
  }
  
  VulkanDescriptorSet& VulkanDescriptorPool::getDescriptorSet(uint32_t index) {
    return this->descriptorSets[index];
  }
}
