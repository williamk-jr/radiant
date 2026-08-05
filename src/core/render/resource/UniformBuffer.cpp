#include "radiant/core/render/resource/UniformBuffer.h"

#include "radiant/core/render/buffers/Buffer.h"
#include "radiant/core/render/resource/ShaderResource.h"
#include "radiant/core/render/vulkan/descriptor/VulkanDescriptorPool.h"

#include <glm/ext/matrix_float4x4.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <string>

namespace Radiant {
	UniformBuffer::UniformBuffer(VulkanMemoryAllocator&     memoryAllocator,
	                             VulkanDescriptorPool&      descriptorPool,
	                             VulkanDescriptorSetLayout& descriptorSetLayout,
	                             VkDeviceSize               size)
	    : Buffer(memoryAllocator, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT),
	      ShaderResource(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, descriptorPool, descriptorSetLayout) {}

	void UniformBuffer::reset() {
		Buffer<void*>::resetOffset();
	}

	void UniformBuffer::append(void* data, size_t size) {
		VkDeviceSize currentOffset = this->getOffset();

		Buffer<void*>::append(data, size);

		this->descriptorBufferWrites.push_back(VkDescriptorBufferInfo{this->buffer->get(), currentOffset, size});
	}

	// TODO Fix uniform buffer writing
	void UniformBuffer::write() {
		this->writeBuffer(this->descriptorBufferWrites);
		this->descriptorBufferWrites.clear();
	}
} // namespace Radiant
