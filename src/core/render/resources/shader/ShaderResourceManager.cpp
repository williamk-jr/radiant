#include "radiant/core/render/resources/shader/ShaderResourceManager.h"

#include "radiant/core/render/vulkan/VulkanDevice.h"

#include <memory>

namespace Radiant {
	ShaderResourceManager::ShaderResourceManager(VulkanDevice& device) {
		this->descriptorPool = std::make_unique<VulkanDescriptorPool>(
		    device,
		    std::vector<VkDescriptorPoolSize>{{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
		                                      {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000}},
		    2000);
	}

	std::unique_ptr<Texture> ShaderResourceManager::allocateTexture(VulkanDevice&              device,
	                                                                VulkanMemoryAllocator&     memoryAllocator,
	                                                                VulkanDescriptorSetLayout& descriptorSetLayout,
	                                                                VulkanCommandPool&         commandPool,
	                                                                VulkanQueue&               queue,
	                                                                void*                      buffer,
	                                                                uint32_t                   width,
	                                                                uint32_t                   height,
	                                                                uint32_t                   pixelSize) {
		return std::make_unique<Texture>(Texture{device, memoryAllocator, *this->descriptorPool, descriptorSetLayout,
		                                         commandPool, queue, buffer, width, height, pixelSize});
	}

	std::unique_ptr<UniformBuffer> ShaderResourceManager::allocateUniformBuffer(
	    VulkanMemoryAllocator& memoryAllocator, VulkanDescriptorSetLayout& descriptorSetLayout, VkDeviceSize size) {

		return std::make_unique<UniformBuffer>(memoryAllocator, *this->descriptorPool, descriptorSetLayout, size);
	}
} // namespace Radiant
