#include "radiant/core/render/vulkan/VulkanQueue.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  VulkanQueue::VulkanQueue(VulkanDevice& device, uint32_t queueFamily, uint32_t queueIndex) {
    vkGetDeviceQueue(device.get(), queueFamily, queueIndex, &this->queue);
  }

  void VulkanQueue::submit(std::vector<VulkanCommandBuffer>& commandBuffers) {
    std::vector<VkCommandBufferSubmitInfo> commandBufferSubmitInfos;
    commandBufferSubmitInfos.reserve(commandBuffers.size());

    for (VulkanCommandBuffer& commandBuffer : commandBuffers) {
      VkCommandBufferSubmitInfo commandBufferSubmitInfo{};
      commandBufferSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
      commandBufferSubmitInfo.commandBuffer = commandBuffer.get();

      commandBufferSubmitInfos.push_back(commandBufferSubmitInfo);
    }

    VkSubmitInfo2 submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
    submitInfo.commandBufferInfoCount = commandBufferSubmitInfos.size();
    submitInfo.pCommandBufferInfos = commandBufferSubmitInfos.data();

    vkQueueSubmit2(this->queue, 1, &submitInfo, nullptr);
  }
}
