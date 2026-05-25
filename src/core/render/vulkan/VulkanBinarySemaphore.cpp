#include "radiant/core/render/vulkan/VulkanBinarySemaphore.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  VulkanBinarySemaphore::VulkanBinarySemaphore(VulkanDevice& device, VkSemaphoreCreateFlags flags) :
    VulkanSemaphore(device, {
        VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
        nullptr,
        VK_SEMAPHORE_TYPE_BINARY,
        0
    }, flags) {}
}
