#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace Radiant {
	/* VulkanDescriptorSet
	 *
	 * A wrapper for VkDescriptorSet.
	 * Represents storage for a set of descriptors.
	 */
	class VulkanDescriptorSet {
		public:
			/*
			 * @param VkDescriptorSet A raw descriptor set.
			 */
			VulkanDescriptorSet(VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorPool descriptorPool);
			VulkanDescriptorSet(const VulkanDescriptorSet&)            = delete;
			VulkanDescriptorSet& operator=(const VulkanDescriptorSet&) = delete;

			VulkanDescriptorSet(VulkanDescriptorSet&&) noexcept;
			VulkanDescriptorSet& operator=(VulkanDescriptorSet&&) noexcept = default;
			~VulkanDescriptorSet();

			/*
			 * @return A raw VkDescriptorSet.
			 */
			VkDescriptorSet get();

			void update(uint32_t                            descriptorBindingIndex,
			            uint32_t                            descriptorArrayElement,
			            VkDescriptorType                    descriptorType,
			            std::vector<VkDescriptorBufferInfo> bufferInfo);

			void update(uint32_t                           descriptorBindingIndex,
			            uint32_t                           descriptorArrayElement,
			            VkDescriptorType                   descriptorType,
			            std::vector<VkDescriptorImageInfo> bufferInfo);

			void update(uint32_t                  descriptorBindingIndex,
			            uint32_t                  descriptorArrayElement,
			            VkDescriptorType          descriptorType,
			            std::vector<VkBufferView> bufferInfo);

		private:
			VkDescriptorSet  descriptorSet;
			VkDescriptorPool descriptorPool;
			VkDevice         device;
	};
} // namespace Radiant
