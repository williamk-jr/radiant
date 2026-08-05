#pragma once

#include "radiant/core/render/resources/shader/Texture.h"
#include "radiant/core/render/resources/shader/UniformBuffer.h"
#include "radiant/core/render/vulkan/descriptor/VulkanDescriptorPool.h"

#include <memory>

namespace Radiant {
	class ShaderResourceManager {
		public:
			ShaderResourceManager(VulkanDevice& device);

			std::unique_ptr<Texture> allocateTexture(VulkanDevice&              device,
			                                         VulkanMemoryAllocator&     memoryAllocator,
			                                         VulkanDescriptorSetLayout& descriptorSetLayout,
			                                         VulkanCommandPool&         commandPool,
			                                         VulkanQueue&               queue,
			                                         void*                      buffer,
			                                         uint32_t                   width,
			                                         uint32_t                   height,
			                                         uint32_t                   pixelSize);

			std::unique_ptr<UniformBuffer> allocateUniformBuffer(VulkanMemoryAllocator&     memoryAllocator,
			                                                     VulkanDescriptorSetLayout& descriptorSetLayout,
			                                                     VkDeviceSize               size);

		private:
			std::unique_ptr<VulkanDescriptorPool> descriptorPool;
	};
} // namespace Radiant
