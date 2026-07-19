#include "radiant/core/render/vulkan/VulkanQueue.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  VulkanQueue::VulkanQueue(VulkanDevice& device, uint32_t queueFamily, uint32_t queueIndex) {
    vkGetDeviceQueue(device.get(), queueFamily, queueIndex, &this->queue);
  }

  VulkanQueue::VulkanQueue(VulkanQueue&& other) noexcept :
    queue(other.queue) {
    other.queue = nullptr;
  }

  void VulkanQueue::submit(std::vector<VulkanCommandBuffer*>& commandBuffers, std::vector<VulkanSemaphoreSubmitInfo> waitSemaphores, std::vector<VulkanSemaphoreSubmitInfo> signalSemaphores, VulkanFence* fence) {
    std::vector<VkCommandBufferSubmitInfo> commandBufferSubmitInfos;
    commandBufferSubmitInfos.reserve(commandBuffers.size());

    for (VulkanCommandBuffer* commandBuffer : commandBuffers) {
      VkCommandBufferSubmitInfo commandBufferSubmitInfo{};
      commandBufferSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
      commandBufferSubmitInfo.commandBuffer = commandBuffer->get();

      commandBufferSubmitInfos.push_back(commandBufferSubmitInfo);
    }

    VkSubmitInfo2 submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
    submitInfo.commandBufferInfoCount = commandBufferSubmitInfos.size();
    submitInfo.pCommandBufferInfos = commandBufferSubmitInfos.data();
    
    // Wait semaphores
    std::vector<VkSemaphoreSubmitInfo> waitSemaphoreInfos;
    if (!waitSemaphores.empty()) {
      waitSemaphoreInfos.reserve(waitSemaphores.size());

      for (VulkanSemaphoreSubmitInfo& i : waitSemaphores) {
        VkSemaphoreSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
        submitInfo.semaphore = i.semaphore->get();
        submitInfo.stageMask = i.flags;
        submitInfo.value = i.value;
        
        waitSemaphoreInfos.push_back(submitInfo);
      }

      submitInfo.waitSemaphoreInfoCount = waitSemaphoreInfos.size();
      submitInfo.pWaitSemaphoreInfos = waitSemaphoreInfos.data();
    }
    
    // Signal semaphores
    std::vector<VkSemaphoreSubmitInfo> signalSemaphoreInfos;
    if (!waitSemaphores.empty()) {
      signalSemaphoreInfos.reserve(signalSemaphores.size());

      for (VulkanSemaphoreSubmitInfo& i : signalSemaphores) {
        VkSemaphoreSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
        submitInfo.semaphore = i.semaphore->get();
        submitInfo.stageMask = i.flags;
        submitInfo.value = i.value;
        
        signalSemaphoreInfos.push_back(submitInfo);
      }

      submitInfo.signalSemaphoreInfoCount = signalSemaphoreInfos.size();
      submitInfo.pSignalSemaphoreInfos = signalSemaphoreInfos.data();
    }

    //Logger::info(std::to_string(waitSemaphores[0].size()));
    if (fence == nullptr) {
      vkQueueSubmit2(this->queue, 1, &submitInfo, nullptr);
    } else {
      vkQueueSubmit2(this->queue, 1, &submitInfo, fence->get());
    }
  }
  
  void VulkanQueue::submit(VulkanCommandBuffer& commandBuffer, std::vector<VulkanSemaphoreSubmitInfo> waitSemaphores, std::vector<VulkanSemaphoreSubmitInfo> signalSemaphores, VulkanFence* fence) {
    std::vector<VulkanCommandBuffer*> cb{&commandBuffer};
    this->submit(cb, waitSemaphores, signalSemaphores, fence);
  }
  //
  //void VulkanQueue::submit(VulkanCommandBuffer& commandBuffer, VulkanSemaphoreSubmitInfo* waitSemaphore, VulkanSemaphoreSubmitInfo* signalSemaphore, VulkanFence& fence) {
  //  std::vector<VulkanSemaphoreSubmitInfo> ws{*waitSemaphore};
  //  std::vector<VulkanSemaphoreSubmitInfo> ss{*signalSemaphore};
  //  this->submit(commandBuffer, ws, ss, fence);
  //}

  void VulkanQueue::present(VulkanSwapchain& swapchain, std::vector<uint32_t> imageIndicies, std::vector<VulkanSemaphore*>& waitSemaphores) {
    std::vector<VkSemaphore> rawSemaphore;
    rawSemaphore.reserve(waitSemaphores.size());

    for (VulkanSemaphore* semaphore : waitSemaphores) {
      rawSemaphore.emplace_back(semaphore->get());
    }

    std::vector<VkSwapchainKHR> swapchains{swapchain.get()}; // TODO Expand to allow more than once swapchain.

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = rawSemaphore.data();
    presentInfo.pImageIndices = imageIndicies.data();
    presentInfo.swapchainCount = swapchains.size();
    presentInfo.pSwapchains = swapchains.data();

    vkQueuePresentKHR(this->queue, &presentInfo);
  }
  
  void VulkanQueue::present(VulkanSwapchain& swapchain, std::vector<uint32_t> imageIndicies, VulkanSemaphore& waitSemaphore) {
    std::vector<VulkanSemaphore*> rawSemaphores{&waitSemaphore};
    this->present(swapchain, imageIndicies, rawSemaphores);
  }

  void VulkanQueue::waitIdle() {
    vkQueueWaitIdle(this->queue);
  }
}
