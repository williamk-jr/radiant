#include "radiant/core/render/vulkan/VulkanImage.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  VulkanImage::VulkanImage(VulkanMemoryAllocator& allocator, VkExtent3D extent) : 
    VulkanImage(allocator, extent, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT) {}

  VulkanImage::VulkanImage(VulkanMemoryAllocator& allocator, VkExtent3D extent, VkFormat format, VkImageUsageFlags flags) : 
    memoryAllocator(allocator.get()), extent(extent) {

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent = extent;
    imageInfo.format = format;
    imageInfo.arrayLayers = 1;
    imageInfo.mipLevels = 1;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = flags;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //imageInfo.flags = 0;

    //vkCreateImage(device.get(), &imageInfo, nullptr, &this->image);
    VmaAllocationCreateInfo allocationInfo{};
    allocationInfo.usage = VMA_MEMORY_USAGE_AUTO;

    Validation::verify(
      vmaCreateImage(allocator.get(), &imageInfo, &allocationInfo, &this->image, &this->imageMemory, nullptr)
    );
  }
  
  VulkanImage::VulkanImage(VkImage image, VkExtent2D extent) : memoryAllocator(nullptr) {
    this->extent = {extent.width, extent.height, 1};
    this->image = image;
  }

  VulkanImage::VulkanImage(VulkanImage&& other) noexcept :
      image(other.image), extent(other.extent), imageMemory(other.imageMemory), memoryAllocator(other.memoryAllocator) {
    other.image = nullptr;
    other.imageMemory = nullptr;
  }

  VulkanImage::~VulkanImage() {
    //vkDestroyImage(this->device.get(), this->image, nullptr);
    if (this->memoryAllocator != nullptr) {
      vmaDestroyImage(this->memoryAllocator, this->image, this->imageMemory);
    }
  }

  VkImage VulkanImage::get() {
    return this->image;
  }
  
  VkExtent3D VulkanImage::getExtent() {
    return this->extent;
  }
}
