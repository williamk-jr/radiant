#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

namespace Radiant {
	class VulkanPipeline {
		public:
			VulkanPipeline(VkDevice device, VkPipelineBindPoint bindPoint, VkPipeline pipeline, VkPipelineLayout layout,
			               VkPipelineCache cache, std::vector<VkShaderModule> shaderModules);
			VulkanPipeline(const VulkanPipeline&) = delete;
			VulkanPipeline& operator=(const VulkanPipeline&) = delete;

			VulkanPipeline(VulkanPipeline&&) noexcept;
			VulkanPipeline& operator=(VulkanPipeline&&) noexcept = default;
			~VulkanPipeline();

			VkPipeline get();
			VkPipelineLayout getLayout();
			VkPipelineBindPoint getBindPoint();
			VkPipelineCache getCache();

		private:
			VkPipeline pipeline;
			VkPipelineBindPoint bindPoint;
			VkPipelineLayout layout;
			VkPipelineCache cache;
			std::vector<VkShaderModule> shaderModules;
			VkDevice device;
	};
} // namespace Radiant
