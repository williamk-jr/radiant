#pragma once
#include "radiant/core/render/buffers/Buffer.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  class IndexBuffer : public Buffer<uint16_t> {
    public:
      IndexBuffer(VulkanMemoryAllocator& memoryAllocator, VkDeviceSize size);
  };
}
