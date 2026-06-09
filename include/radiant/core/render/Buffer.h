#pragma once

#include "radiant/core/render/vulkan/VulkanBuffer.h"
#include <cstddef>
#include <memory>
namespace Radiant {
  template<typename T>
  class Buffer {
    public:
      Buffer(VulkanMemoryAllocator& memoryAllocator, VkDeviceSize size, VkBufferUsageFlagBits usage) {
        this->buffer = std::make_unique<VulkanBuffer>(
          memoryAllocator,
          size,
          usage,
          VK_SHARING_MODE_EXCLUSIVE,
          std::vector<uint32_t>{}
        );
      }

      ~Buffer() {}

      VulkanBuffer& get() {
        return *this->buffer;
      }

      void append(std::vector<T> data) {
        this->buffer->append(data.data(), sizeof(T)*data.size());
      }

      void size() {

      }

    private:
      std::unique_ptr<VulkanBuffer> buffer;
  };
}
