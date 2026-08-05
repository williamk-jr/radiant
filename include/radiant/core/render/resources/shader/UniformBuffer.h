#pragma once

#include "radiant/core/render/resources/Buffer.h"
#include "radiant/core/render/resources/shader/ShaderResource.h"

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
