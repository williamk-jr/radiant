#pragma once
#include "radiant/core/render/resource/ShaderResource.h"
#include "radiant/core/render/vulkan/VulkanQueue.h"
#include "radiant/core/render/vulkan/VulkanSampler.h"
#include "radiant/core/render/vulkan/descriptor/VulkanDescriptorPool.h"
#include "radiant/core/render/vulkan/descriptor/VulkanDescriptorSet.h"
#include "radiant/core/render/vulkan/descriptor/VulkanDescriptorSetLayout.h"
#include "radiant/core/render/vulkan/resource/VulkanImage.h"
#include "radiant/core/render/vulkan/resource/VulkanImageView.h"

#include <memory>

namespace Radiant {
	class ShaderResourceManager;

	class Texture : public ShaderResource {
			friend ShaderResourceManager;

		public:
			Texture(const Texture&)            = delete;
			Texture& operator=(const Texture&) = delete;

			Texture(Texture&&) noexcept;
			Texture& operator=(Texture&&) noexcept = default;

		private:
			Texture(VulkanDevice&              device,
			        VulkanMemoryAllocator&     memoryAllocator,
			        VulkanDescriptorPool&      descriptorPool,
			        VulkanDescriptorSetLayout& descriptorSetLayout,
			        VulkanCommandPool&         commandPool,
			        VulkanQueue&               queue,
			        void*                      buffer,
			        uint32_t                   width,
			        uint32_t                   height,
			        uint32_t                   pixelSize);

			std::unique_ptr<VulkanImage>     image;
			std::unique_ptr<VulkanImageView> imageView;
			std::unique_ptr<VulkanSampler>   sampler;
	};
} // namespace Radiant
