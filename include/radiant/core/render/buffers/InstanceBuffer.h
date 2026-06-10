#pragma once
#include "radiant/core/render/buffers/Buffer.h"
#include "radiant/core/render/Instance.h"
#include "radiant/core/render/Vertex.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  class InstanceBuffer : public Buffer<Instance> {
    public:
      InstanceBuffer(VulkanMemoryAllocator& memoryAllocator, VkDeviceSize size);
  };
}
