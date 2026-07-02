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

    VmaAllocationCreateInfo allocationCreateInfo{};
    allocationCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;
    allocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;

    vmaCreateBuffer(memoryAllocator.get(), &bufferInfo, &allocationCreateInfo, &this->buffer, &this->allocation, &this->allocationInfo); 
  }
  
  VulkanBuffer::VulkanBuffer(VulkanBuffer&& other) noexcept : 
    buffer(other.buffer), size(other.size), allocation(other.allocation), allocationInfo(other.allocationInfo), memoryAllocator(other.memoryAllocator) {
    other.buffer = nullptr;
    other.allocation = nullptr;
    other.allocationInfo = {};
  }

  VulkanBuffer::~VulkanBuffer() {
    vmaDestroyBuffer(this->memoryAllocator, this->buffer, this->allocation);
  }

  VkBuffer VulkanBuffer::get() {
    return this->buffer;
  }

  void VulkanBuffer::append(void* data, size_t size) {
    memcpy(((char*)this->allocationInfo.pMappedData+this->offset), data, size);
    this->offset += size;
  }

  VkDeviceSize VulkanBuffer::getOffset() {
    return this->offset;
  }
  
  VkDeviceSize VulkanBuffer::getSize() {
    return this->size;
  }
  
  void VulkanBuffer::resetOffset() {
    this->offset = 0;
  }
}
