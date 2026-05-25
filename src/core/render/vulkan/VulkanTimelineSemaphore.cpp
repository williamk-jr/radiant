
#include "radiant/core/render/vulkan/VulkanTimelineSemaphore.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include <cstdint>
#include <vulkan/vulkan_core.h>

namespace Radiant {
  VulkanTimelineSemaphore::VulkanTimelineSemaphore(VulkanDevice& device, VkSemaphoreCreateFlags flags, uint64_t initialValue) :
    VulkanSemaphore(device, {
        VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
        nullptr,
        VK_SEMAPHORE_TYPE_TIMELINE,
        initialValue
    }, flags) {}
    
  uint64_t VulkanTimelineSemaphore::getCounterValue() {
    uint64_t value = 0;
    vkGetSemaphoreCounterValue(this->device.get(), this->get(), &value);
    return value;
  }
  
  void VulkanTimelineSemaphore::signal(uint64_t value) {
    VkSemaphoreSignalInfo signalInfo{};
    signalInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO;
    signalInfo.semaphore = this->get();
    signalInfo.value = value;

    Validation::verify(
      vkSignalSemaphore(this->device.get(), &signalInfo)
    );
  }

  void VulkanTimelineSemaphore::waitSemaphores(VulkanDevice& device, std::vector<VulkanTimelineSemaphore>& semaphores, std::vector<uint64_t>& values, VkSemaphoreWaitFlags flags, uint64_t timeout) {
    std::vector<VkSemaphore> rawSemaphores(semaphores.size());
    
    for (VulkanSemaphore& semaphore : semaphores) {
      rawSemaphores.push_back(semaphore.get());
    }

    VkSemaphoreWaitInfo waitInfo{};
    waitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO;
    waitInfo.semaphoreCount = rawSemaphores.size();
    waitInfo.pSemaphores = rawSemaphores.data();
    waitInfo.pValues = values.data();
    waitInfo.flags = flags;
    
    Validation::verify(
      vkWaitSemaphores(device.get(), &waitInfo, timeout)
    );
  }
}
