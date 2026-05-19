#define VMA_IMPLEMENTATION // Very required, read vma docs.
#include "radiant/core/render/vulkan/VulkanMemoryAllocator.h"

namespace Radiant {
  VulkanMemoryAllocator::VulkanMemoryAllocator(VulkanInstance& instance, VulkanPhysicalDevice& physicalDevice, VulkanDevice& device) {
    VmaAllocatorCreateInfo allocatorInfo{};
    allocatorInfo.instance = instance.get();
    allocatorInfo.physicalDevice = physicalDevice.get();
    allocatorInfo.device = device.get();
    //allocatorInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
    Validation::verify(
      vmaCreateAllocator(&allocatorInfo, &this->memoryAllocator)
    );
  }
}
