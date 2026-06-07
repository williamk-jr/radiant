#include "radiant/core/render/vulkan/VulkanBuffer.h"
#include <cstring>

namespace Radiant {
  VulkanBuffer::VulkanBuffer(VulkanMemoryAllocator& memoryAllocator, VkDeviceSize size, VkBufferUsageFlagBits usage, VkSharingMode sharingMode, std::vector<uint32_t> queueFamilyIndicies) : memoryAllocator(memoryAllocator.get()), size(size) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = sharingMode;
    bufferInfo.queueFamilyIndexCount = queueFamilyIndicies.size();
    bufferInfo.pQueueFamilyIndices = queueFamilyIndicies.data();
    bufferInfo.flags = 0;

    VmaAllocationCreateInfo allocationInfo{};
    allocationInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT;
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

  void VulkanBuffer::append(void* data, size_t size) {
    memcpy(((char*)this->allocationInfo.pMappedData+this->offset), data, size);
    this->offset += size;
  }
}
