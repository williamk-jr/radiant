
#include "radiant/core/render/resources/InstanceBuffer.h"

namespace Radiant {
	InstanceBuffer::InstanceBuffer(VulkanMemoryAllocator& memoryAllocator, VkDeviceSize size)
	    : Buffer(memoryAllocator, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) {}
} // namespace Radiant
