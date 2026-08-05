#pragma once

#include "radiant/core/render/buffers/Buffer.h"
#include "radiant/core/render/resource/ShaderResource.h"
#include "radiant/core/render/vulkan/resource/VulkanBuffer.h"

#include <memory>

namespace Radiant {
	class UniformBuffer : public ShaderResource, protected Buffer<void*> {
		public:
			UniformBuffer(VulkanMemoryAllocator&     memoryAllocator,
			              VulkanDescriptorPool&      descriptorPool,
			              VulkanDescriptorSetLayout& descriptorSetLayout,
			              VkDeviceSize               size);

			void reset();
			void append(void* data, size_t size);
			void write();

		private:
			std::vector<VkDescriptorBufferInfo> descriptorBufferWrites;
	};
} // namespace Radiant
