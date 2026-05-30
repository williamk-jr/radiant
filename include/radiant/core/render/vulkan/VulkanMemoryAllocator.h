#pragma once
#include <vma/vk_mem_alloc.h>

#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanInstance.h"
#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"

namespace Radiant {
  class VulkanMemoryAllocator {
    public:
      VulkanMemoryAllocator(VulkanInstance& instance, VulkanPhysicalDevice& physicalDevice, VulkanDevice& device);
      ~VulkanMemoryAllocator();
      VmaAllocator& get();

    private:
      VmaAllocator memoryAllocator;
  };
}
