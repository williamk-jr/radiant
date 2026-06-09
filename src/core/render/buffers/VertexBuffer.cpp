#include "radiant/core/render/buffers/VertexBuffer.h"

namespace Radiant {
  VertexBuffer::VertexBuffer(VulkanMemoryAllocator& memoryAllocator, VkDeviceSize size) : 
    Buffer(memoryAllocator, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) {}
}
