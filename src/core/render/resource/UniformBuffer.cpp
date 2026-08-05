#include "radiant/core/render/resource/UniformBuffer.h"

#include "radiant/core/render/resource/ShaderResource.h"
#include "radiant/core/render/vulkan/descriptor/VulkanDescriptorPool.h"

namespace Radiant {
	UniformBuffer::UniformBuffer(VulkanMemoryAllocator&     memoryAllocator,
	                             VulkanDescriptorPool&      descriptorPool,
	                             VulkanDescriptorSetLayout& descriptorSetLayout,
	                             VkDeviceSize               size)
	    : Buffer(memoryAllocator, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT),
	      ShaderResource(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, descriptorPool, descriptorSetLayout) {}

	void UniformBuffer::write() {
		this->writeBuffer({VkDescriptorBufferInfo{this->getBuffer().get(), 0, this->getOffset()}});
		// this->descriptorBufferWrites.clear();
	}
} // namespace Radiant
