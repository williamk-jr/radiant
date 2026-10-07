#include "radiant/core/render/vulkan/descriptor/VulkanDescriptorSet.h"

#include "radiant/core/render/vulkan/descriptor/VulkanDescriptorPool.h"

#include <algorithm>
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Radiant {
	VulkanDescriptorSet::VulkanDescriptorSet(VkDevice         device,
	                                         VkDescriptorSet  descriptorSet,
	                                         VkDescriptorPool descriptorPool)
	    : device(device), descriptorSet(descriptorSet), descriptorPool(descriptorPool) {}

	VulkanDescriptorSet::VulkanDescriptorSet(VulkanDescriptorSet&& other) noexcept
	    : device(other.device), descriptorSet(other.descriptorSet), descriptorPool(other.descriptorPool) {
		other.descriptorSet = nullptr;
	}

	void VulkanDescriptorSet::update(uint32_t                            descriptorBindingIndex,
	                                 uint32_t                            descriptorArrayElement,
	                                 VkDescriptorType                    descriptorType,
	                                 std::vector<VkDescriptorBufferInfo> bufferInfo) {

		VkWriteDescriptorSet rawDescriptorWrite{};
		rawDescriptorWrite.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		rawDescriptorWrite.dstSet          = this->descriptorSet;
		rawDescriptorWrite.dstArrayElement = descriptorArrayElement;
		rawDescriptorWrite.dstBinding      = descriptorBindingIndex;
		rawDescriptorWrite.descriptorType  = descriptorType;
		rawDescriptorWrite.pNext           = nullptr;

		rawDescriptorWrite.descriptorCount = (uint32_t)bufferInfo.size();
		rawDescriptorWrite.pBufferInfo     = bufferInfo.data();

		vkUpdateDescriptorSets(this->device, 1, &rawDescriptorWrite, 0, nullptr);
	}

	void VulkanDescriptorSet::update(uint32_t                           descriptorBindingIndex,
	                                 uint32_t                           descriptorArrayElement,
	                                 VkDescriptorType                   descriptorType,
	                                 std::vector<VkDescriptorImageInfo> imageInfo) {

		VkWriteDescriptorSet rawDescriptorWrite{};
		rawDescriptorWrite.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		rawDescriptorWrite.dstSet          = this->descriptorSet;
		rawDescriptorWrite.dstArrayElement = descriptorArrayElement;
		rawDescriptorWrite.dstBinding      = descriptorBindingIndex;
		rawDescriptorWrite.descriptorType  = descriptorType;
		rawDescriptorWrite.pNext           = nullptr;

		rawDescriptorWrite.descriptorCount = (uint32_t)imageInfo.size();
		rawDescriptorWrite.pImageInfo      = imageInfo.data();

		vkUpdateDescriptorSets(this->device, 1, &rawDescriptorWrite, 0, nullptr);
	}

	void VulkanDescriptorSet::update(uint32_t                  descriptorBindingIndex,
	                                 uint32_t                  descriptorArrayElement,
	                                 VkDescriptorType          descriptorType,
	                                 std::vector<VkBufferView> texelBufferViews) {

		VkWriteDescriptorSet rawDescriptorWrite{};
		rawDescriptorWrite.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		rawDescriptorWrite.dstSet          = this->descriptorSet;
		rawDescriptorWrite.dstArrayElement = descriptorArrayElement;
		rawDescriptorWrite.dstBinding      = descriptorBindingIndex;
		rawDescriptorWrite.descriptorType  = descriptorType;
		rawDescriptorWrite.pNext           = nullptr;

		rawDescriptorWrite.descriptorCount  = (uint32_t)texelBufferViews.size();
		rawDescriptorWrite.pTexelBufferView = texelBufferViews.data();

		vkUpdateDescriptorSets(this->device, 1, &rawDescriptorWrite, 0, nullptr);
	}

	VulkanDescriptorSet::~VulkanDescriptorSet() {
		vkFreeDescriptorSets(this->device, this->descriptorPool, 1, &this->descriptorSet);
	}

	VkDescriptorSet VulkanDescriptorSet::get() {
		return this->descriptorSet;
	}
} // namespace Radiant
