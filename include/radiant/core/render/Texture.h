#pragma once
#include "radiant/core/render/vulkan/VulkanDescriptorPool.h"
#include "radiant/core/render/vulkan/VulkanDescriptorSet.h"
#include "radiant/core/render/vulkan/VulkanDescriptorSetLayout.h"
#include "radiant/core/render/vulkan/VulkanImage.h"
#include "radiant/core/render/vulkan/VulkanImageView.h"
#include "radiant/core/render/vulkan/VulkanQueue.h"
#include "radiant/core/render/vulkan/VulkanSampler.h"
#include <cstddef>
#include <filesystem>
#include <memory>
#include <vector>

namespace Radiant {
  class Renderer;

  class Texture {
    friend Renderer;

    public:
      Texture(const Texture&) = delete;
      Texture& operator=(const Texture&) = delete;

      Texture(Texture&&) noexcept;
      Texture& operator=(Texture&&) noexcept = default;

    private:
      Texture(VulkanDevice& device, VulkanMemoryAllocator& memoryAllocator, VulkanDescriptorPool& descriptorPool, VulkanCommandPool& commandPool, VulkanQueue& queue, void* buffer, uint32_t width, uint32_t height, uint32_t pixelSize);
      
      std::unique_ptr<VulkanImage> image;
      std::unique_ptr<VulkanImageView> imageView;
      std::unique_ptr<VulkanSampler> sampler;
      std::unique_ptr<VulkanDescriptorSetLayout> descriptorSetLayout;
      std::unique_ptr<VulkanDescriptorSet> descriptorSet;
  };
}
