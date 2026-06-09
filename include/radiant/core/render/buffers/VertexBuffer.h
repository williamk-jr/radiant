#pragma once
#include "radiant/core/render/Buffer.h"
#include "radiant/core/render/Vertex.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  class VertexBuffer : public Buffer<Vertex> {
    public:
      VertexBuffer(VulkanMemoryAllocator& memoryAllocator, VkDeviceSize size);
  };
}
