#include "radiant/core/render/vulkan/VulkanSemaphore.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  VulkanSemaphore::VulkanSemaphore(VulkanDevice& device, VkSemaphoreTypeCreateInfo type, VkSemaphoreCreateFlags flags) : device(device) {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.flags = flags;
    semaphoreInfo.pNext = &type;
    Validation::verify(
      vkCreateSemaphore(device.get(), &semaphoreInfo, nullptr, &this->semaphore)
    );
  }
  
  VulkanSemaphore::~VulkanSemaphore() {
    vkDestroySemaphore(this->device.get(), this->semaphore, nullptr);
  }

  VkSemaphore VulkanSemaphore::get() const {
    return this->semaphore;
  }
  
}
