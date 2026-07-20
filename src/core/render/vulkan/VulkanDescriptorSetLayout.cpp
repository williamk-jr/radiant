#include "radiant/core/render/vulkan/VulkanDescriptorSetLayout.h"
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Radiant {
  VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(VulkanDevice& device, std::vector<VkDescriptorSetLayoutBinding> descriptorBindings, VkDescriptorSetLayoutCreateFlags flags) :
    device(device.get()) {
    std::vector<VkDescriptorBindingFlags> bindingFlags;
    for (int i = 0; i < descriptorBindings.size(); i++) {
      bindingFlags.push_back(VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT);
    }

    VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsInfo{};
    bindingFlagsInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
    bindingFlagsInfo.bindingCount = bindingFlags.size();
    bindingFlagsInfo.pBindingFlags = bindingFlags.data();

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
    descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.bindingCount = descriptorBindings.size();
    descriptorSetLayoutInfo.pBindings = descriptorBindings.data();
    descriptorSetLayoutInfo.flags = flags; //VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;
    descriptorSetLayoutInfo.pNext = flags & VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT ? &bindingFlagsInfo : nullptr;

    Validation::verify(
        vkCreateDescriptorSetLayout(device.get(), &descriptorSetLayoutInfo, nullptr, &this->layout)
    );
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
