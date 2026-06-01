#include "radiant/core/render/vulkan/VulkanImageView.h"
#include "radiant/core/render/vulkan/VulkanImage.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
    VulkanImageView::VulkanImageView(VulkanDevice& device, VulkanImage& image, VkComponentMapping componentMapping, VkImageSubresourceRange subresourceRange, VkImageViewCreateFlags flags) : device(device) {
    VkImageViewCreateInfo imageViewInfo{};
    imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewInfo.image = image.get();
    imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewInfo.format = VK_FORMAT_B8G8R8A8_SRGB;
    imageViewInfo.components = componentMapping;
    imageViewInfo.subresourceRange = subresourceRange;
    imageViewInfo.flags = flags;

    vkCreateImageView(device.get(), &imageViewInfo, nullptr, &this->imageView);
  }
  VulkanImageView::VulkanImageView(VulkanImageView&& other) noexcept :
    imageView(other.imageView), device(other.device) {
    other.imageView = nullptr;
  }
  
  VulkanImageView::~VulkanImageView() {
    vkDestroyImageView(this->device.get(), this->imageView, nullptr);
  }
  
  VkImageView VulkanImageView::get() {
    return this->imageView;
  }
}
