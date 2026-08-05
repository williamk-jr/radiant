#pragma once

#include "radiant/core/render/vulkan/VulkanCommandBuffer.h"
#include "radiant/core/render/vulkan/descriptor/VulkanDescriptorPool.h"
#include "radiant/core/render/vulkan/descriptor/VulkanDescriptorSet.h"

#include <memory>
#include <vulkan/vulkan_core.h>

namespace Radiant {
	class ShaderResource {
		public:
			ShaderResource(VkDescriptorType           descriptorType,
			               VulkanDescriptorPool&      descriptorPool,
			               VulkanDescriptorSetLayout& descriptorSetLayout);
			ShaderResource(const ShaderResource&)            = delete;
			ShaderResource& operator=(const ShaderResource&) = delete;

			ShaderResource(ShaderResource&&) noexcept;
			ShaderResource& operator=(ShaderResource&&) noexcept = default;

			VulkanDescriptorSet& getDescriptorSet();
			void                 bind(VulkanPipeline& pipeline, VulkanCommandBuffer& commandBuffer, uint32_t firstSet);

		protected:
			std::unique_ptr<VulkanDescriptorSet> descriptorSet;
			VkDescriptorType                     descriptorType;

			void writeBuffer(std::vector<VkDescriptorBufferInfo> bufferInfo);
			void writeImage(std::vector<VkDescriptorImageInfo> imageInfo);
	};
} // namespace Radiant
