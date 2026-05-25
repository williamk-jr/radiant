#include "radiant/core/render/vulkan/VulkanQueue.h"
#include "radiant/core/render/vulkan/VulkanFence.h"
#include "radiant/core/render/vulkan/VulkanSemaphore.h"
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

  void VulkanQueue::submit(std::vector<VulkanCommandBuffer>& commandBuffers, VulkanFence& fence) {
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

    vkQueueSubmit2(this->queue, 1, &submitInfo, fence.get());
  }

  void VulkanQueue::submit(std::vector<VulkanCommandBuffer>& commandBuffers, std::vector<VulkanSemaphoreSubmitInfo>& waitSemaphores, std::vector<VulkanSemaphoreSubmitInfo>& signalSemaphores) {
    std::vector<VkCommandBufferSubmitInfo> commandBufferSubmitInfos;
    commandBufferSubmitInfos.reserve(commandBuffers.size());

    for (VulkanCommandBuffer& commandBuffer : commandBuffers) {
      VkCommandBufferSubmitInfo commandBufferSubmitInfo{};
      commandBufferSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
      commandBufferSubmitInfo.commandBuffer = commandBuffer.get();

      commandBufferSubmitInfos.push_back(commandBufferSubmitInfo);
    }

    std::vector<VkSemaphoreSubmitInfo> waitSemaphoreInfos;
    std::vector<VkSemaphoreSubmitInfo> signalSemaphoreInfos;

    for (VulkanSemaphoreSubmitInfo& i : waitSemaphores) {
      VkSemaphoreSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
      submitInfo.semaphore = i.semaphore.get();
      submitInfo.stageMask = i.flags;
      submitInfo.value = i.value;
      
      waitSemaphoreInfos.push_back(submitInfo);
    }

    for (VulkanSemaphoreSubmitInfo& i : signalSemaphores) {
      VkSemaphoreSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
      submitInfo.semaphore = i.semaphore.get();
      submitInfo.stageMask = i.flags;
      submitInfo.value = i.value;
      
      signalSemaphoreInfos.push_back(submitInfo);
    }

    VkSubmitInfo2 submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
    submitInfo.commandBufferInfoCount = commandBufferSubmitInfos.size();
    submitInfo.pCommandBufferInfos = commandBufferSubmitInfos.data();
    submitInfo.waitSemaphoreInfoCount = waitSemaphoreInfos.size();
    submitInfo.pWaitSemaphoreInfos = waitSemaphoreInfos.data();
    submitInfo.signalSemaphoreInfoCount = signalSemaphoreInfos.size();
    submitInfo.pSignalSemaphoreInfos = signalSemaphoreInfos.data();

    vkQueueSubmit2(this->queue, 1, &submitInfo, nullptr);
  }

  void VulkanQueue::submit(std::vector<VulkanCommandBuffer>& commandBuffers, std::vector<VulkanSemaphoreSubmitInfo>& waitSemaphores, std::vector<VulkanSemaphoreSubmitInfo>& signalSemaphores, VulkanFence& fence) {
    std::vector<VkCommandBufferSubmitInfo> commandBufferSubmitInfos;
    commandBufferSubmitInfos.reserve(commandBuffers.size());

    for (VulkanCommandBuffer& commandBuffer : commandBuffers) {
      VkCommandBufferSubmitInfo commandBufferSubmitInfo{};
      commandBufferSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
      commandBufferSubmitInfo.commandBuffer = commandBuffer.get();

      commandBufferSubmitInfos.push_back(commandBufferSubmitInfo);
    }

    std::vector<VkSemaphoreSubmitInfo> waitSemaphoreInfos;
    std::vector<VkSemaphoreSubmitInfo> signalSemaphoreInfos;

    for (VulkanSemaphoreSubmitInfo& i : waitSemaphores) {
      VkSemaphoreSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
      submitInfo.semaphore = i.semaphore.get();
      submitInfo.stageMask = i.flags;
      submitInfo.value = i.value;
      
      waitSemaphoreInfos.push_back(submitInfo);
    }

    for (VulkanSemaphoreSubmitInfo& i : signalSemaphores) {
      VkSemaphoreSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
      submitInfo.semaphore = i.semaphore.get();
      submitInfo.stageMask = i.flags;
      submitInfo.value = i.value;
      
      signalSemaphoreInfos.push_back(submitInfo);
    }

    VkSubmitInfo2 submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
    submitInfo.commandBufferInfoCount = commandBufferSubmitInfos.size();
    submitInfo.pCommandBufferInfos = commandBufferSubmitInfos.data();
    submitInfo.waitSemaphoreInfoCount = waitSemaphoreInfos.size();
    submitInfo.pWaitSemaphoreInfos = waitSemaphoreInfos.data();
    submitInfo.signalSemaphoreInfoCount = signalSemaphoreInfos.size();
    submitInfo.pSignalSemaphoreInfos = signalSemaphoreInfos.data();

    vkQueueSubmit2(this->queue, 1, &submitInfo, fence.get());
  }
}
