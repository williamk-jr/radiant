#include "radiant/core/render/resources/IndexBuffer.h"

namespace Radiant {
	IndexBuffer::IndexBuffer(VulkanMemoryAllocator& memoryAllocator, VkDeviceSize size)
	    : Buffer(memoryAllocator, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT) {}
} // namespace Radiant
