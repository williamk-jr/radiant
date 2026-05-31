#include "radiant/core/render/vulkan/VulkanFence.h"
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include <cstdint>
#include <vulkan/vulkan_core.h>

namespace Radiant {
  VulkanFence::VulkanFence(VulkanDevice& device, VkFenceCreateFlags flags) : device(device) {
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = flags;

    Validation::verify(
      vkCreateFence(device.get(), &fenceInfo, nullptr, &this->fence)
    );
  }

  VulkanFence::VulkanFence(VulkanFence&& other) noexcept :
    fence(other.fence), device(other.device) {
    other.fence = nullptr;
  }

  VulkanFence::~VulkanFence() {
    vkDestroyFence(this->device.get(), this->fence, nullptr);
  }
  
  VkFence VulkanFence::get() {
    return this->fence;
  }
  
  void VulkanFence::reset() {
    vkResetFences(this->device.get(), 1, &this->fence);
  }

  void VulkanFence::wait(uint32_t timeout) {
    vkWaitForFences(this->device.get(), 1, &this->fence, VK_TRUE, timeout);
  }
  
  bool VulkanFence::isSignaled() {
    return vkGetFenceStatus(this->device.get(), this->fence) == VK_SUCCESS;
  }
  

  void VulkanFence::waitForFences(VulkanDevice& device, std::vector<VulkanFence>& fences, bool waitAll, uint64_t timeout) {
    std::vector<VkFence> rawFences(fences.size());
    for (int i = 0; i < fences.size(); i++) {
      rawFences[i] = fences[i].get();
    }
    vkWaitForFences(device.get(), fences.size(), rawFences.data(), waitAll, timeout);
  }
}
