#include "radiant/core/render/vulkan/VulkanBuffer.h"

namespace Radiant {
  VulkanBuffer::VulkanBuffer(VulkanMemoryAllocator& memoryAllocator, VkDeviceSize size, VkBufferUsageFlagBits usage, VkSharingMode sharingMode, std::vector<uint32_t> queueFamilyIndicies) : memoryAllocator(memoryAllocator.get()) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = sharingMode;
    bufferInfo.queueFamilyIndexCount = queueFamilyIndicies.size();
    bufferInfo.pQueueFamilyIndices = queueFamilyIndicies.data();
    bufferInfo.flags = 0;

    VmaAllocationCreateInfo allocationInfo{};
    allocationInfo.usage = VMA_MEMORY_USAGE_AUTO;

    vmaCreateBuffer(memoryAllocator.get(), &bufferInfo, &allocationInfo, &this->buffer, &this->allocation, nullptr); 
  }
  
  VulkanBuffer::VulkanBuffer(VulkanBuffer&& other) noexcept : 
    buffer(other.buffer), allocation(other.allocation), memoryAllocator(other.memoryAllocator) {
    other.buffer = nullptr;
    other.allocation = nullptr;
  }

  VulkanBuffer::~VulkanBuffer() {
    vmaDestroyBuffer(this->memoryAllocator, this->buffer, this->allocation);
  }
}
