#include "radiant/core/render/vulkan/VulkanSemaphore.h"

namespace Radiant {
  VulkanSemaphore::VulkanSemaphore(VulkanDevice& device, VkSemaphoreTypeCreateInfo type, VkSemaphoreCreateFlags flags) : device(device.get()) {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.flags = flags;
    semaphoreInfo.pNext = &type;
    Validation::verify(
      vkCreateSemaphore(device.get(), &semaphoreInfo, nullptr, &this->semaphore)
    );
  }

  VulkanSemaphore::VulkanSemaphore(VulkanSemaphore&& other) noexcept :
    semaphore(other.semaphore), device(other.device) {
    other.semaphore = nullptr;
    other.device = nullptr;
  }
  
  VulkanSemaphore::~VulkanSemaphore() {
    vkDestroySemaphore(this->device, this->semaphore, nullptr);
  }

  VkSemaphore VulkanSemaphore::get() const {
    return this->semaphore;
  }
}
