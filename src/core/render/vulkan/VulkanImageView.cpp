#include "radiant/core/render/vulkan/VulkanImageView.h"

namespace Radiant {
    VulkanImageView::VulkanImageView(VulkanDevice& device, VulkanImage& image, VkComponentMapping componentMapping, VkImageSubresourceRange subresourceRange, VkImageViewCreateFlags flags) : device(device.get()) {
    VkImageViewCreateInfo imageViewInfo{};
    imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewInfo.image = image.get();
    imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewInfo.format = VK_FORMAT_B8G8R8A8_SRGB;
    imageViewInfo.components = componentMapping;
    imageViewInfo.subresourceRange = subresourceRange;
    imageViewInfo.flags = flags;

    vkCreateImageView(device.get(), &imageViewInfo, nullptr, &this->imageView);

    this->extent = image.getExtent();
  }
  VulkanImageView::VulkanImageView(VulkanImageView&& other) noexcept :
    imageView(other.imageView), extent(other.extent), device(other.device) {
    other.imageView = nullptr;
  }
  
  VulkanImageView::~VulkanImageView() {
    vkDestroyImageView(this->device, this->imageView, nullptr);
  }
  
  VkImageView VulkanImageView::get() {
    return this->imageView;
  }

  VkExtent3D VulkanImageView::getExtent() {
    return this->extent;
  }
}
