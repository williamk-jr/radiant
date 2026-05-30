#define VMA_IMPLEMENTATION // Very required, read vma docs.
#include "radiant/core/render/vulkan/VulkanMemoryAllocator.h"

namespace Radiant {
  VulkanMemoryAllocator::VulkanMemoryAllocator(VulkanInstance& instance, VulkanPhysicalDevice& physicalDevice, VulkanDevice& device) {
    VmaVulkanFunctions vulkanFunctions{};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocatorInfo{};
    allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_4;
    allocatorInfo.instance = instance.get();
    allocatorInfo.physicalDevice = physicalDevice.get();
    allocatorInfo.device = device.get();
    allocatorInfo.flags = 0;
    allocatorInfo.pVulkanFunctions = &vulkanFunctions;

    Validation::verify(
      vmaCreateAllocator(&allocatorInfo, &this->memoryAllocator)
    );
  }
  
  VulkanMemoryAllocator::~VulkanMemoryAllocator() {
    vmaDestroyAllocator(this->memoryAllocator);
  }

  VmaAllocator& VulkanMemoryAllocator::get() {
    return this->memoryAllocator;
  }
}
