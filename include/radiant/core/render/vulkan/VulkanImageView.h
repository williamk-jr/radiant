#pragma once
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanImage.h"
#include "radiant/core/render/vulkan/VulkanMemoryAllocator.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  class VulkanImageView {
    public:
      VulkanImageView(VulkanDevice& device, VulkanImage& image, VkComponentMapping componentMapping, VkImageSubresourceRange subresourceRange, VkImageViewCreateFlags flags);
      VulkanImageView(const VulkanImageView&) = delete;
      VulkanImageView& operator=(const VulkanImageView&) = delete;

      VulkanImageView(VulkanImageView&&) noexcept;
      VulkanImageView& operator=(VulkanImageView&&) noexcept = default;
      ~VulkanImageView();
      
      VkImageView get();

    private:
      VkImageView imageView;
      VulkanDevice& device;
  };
}
