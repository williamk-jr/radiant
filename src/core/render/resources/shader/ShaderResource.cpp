#include "radiant/core/render/resources/shader/ShaderResource.h"

#include "radiant/core/render/vulkan/descriptor/VulkanDescriptorPool.h"
#include "radiant/core/render/vulkan/descriptor/VulkanDescriptorSet.h"
#include "radiant/core/render/vulkan/descriptor/VulkanDescriptorSetLayout.h"

#include <vulkan/vulkan_core.h>

namespace Radiant {
	ShaderResource::ShaderResource(VkDescriptorType           descriptorType,
	                               VulkanDescriptorPool&      descriptorPool,
	                               VulkanDescriptorSetLayout& descriptorSetLayout)
	    : descriptorType(descriptorType) {
		this->descriptorSet =
		    std::make_unique<VulkanDescriptorSet>(descriptorPool.allocateDescriptorSet(descriptorSetLayout));
	}

	ShaderResource::ShaderResource(ShaderResource&& other) noexcept
	    : descriptorSet(std::move(other.descriptorSet)), descriptorType(other.descriptorType) {}

	VulkanDescriptorSet& ShaderResource::getDescriptorSet() {
		return *this->descriptorSet;
	}

	void ShaderResource::bind(VulkanPipeline& pipeline, VulkanCommandBuffer& commandBuffer, uint32_t firstSet) {
		commandBuffer.bindDescriptorSet(pipeline, firstSet, *this->descriptorSet);
	}

	void ShaderResource::writeBuffer(std::vector<VkDescriptorBufferInfo> bufferInfo) {
		this->descriptorSet->update(0, 0, this->descriptorType, bufferInfo);
	}

	void ShaderResource::writeImage(std::vector<VkDescriptorImageInfo> imageInfo) {
		this->descriptorSet->update(0, 0, this->descriptorType, imageInfo);
	}
} // namespace Radiant
