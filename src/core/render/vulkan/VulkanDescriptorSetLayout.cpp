#include "radiant/core/render/vulkan/VulkanDescriptorSetLayout.h"
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Radiant {
  VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(VulkanDevice& device, std::vector<VkDescriptorSetLayoutBinding> descriptorBindings) :
    device(device.get()) {
    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
    descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.bindingCount = descriptorBindings.size();
    descriptorSetLayoutInfo.pBindings = descriptorBindings.data();
    descriptorSetLayoutInfo.flags = 0;

    vkCreateDescriptorSetLayout(device.get(), &descriptorSetLayoutInfo, nullptr, &this->layout);
  }
  
  VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(VulkanDescriptorSetLayout&& other) noexcept :
    layout(other.layout), device(other.device) {
    other.layout = nullptr; 
  }

  VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(this->device, this->layout, nullptr);
  }

  VkDescriptorSetLayout VulkanDescriptorSetLayout::get() {
    return this->layout;
  }
}
