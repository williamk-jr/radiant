#pragma once
#include <cstdint>
#include "radiant/core/render/vulkan/VulkanCommandPool.h"
#include "radiant/core/render/vulkan/VulkanFence.h"
#include "radiant/core/render/vulkan/VulkanSemaphore.h"
#include "radiant/core/render/vulkan/VulkanSwapchain.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  struct VulkanSemaphoreSubmitInfo {
    VulkanSemaphore* semaphore;
    VkPipelineStageFlags2 flags;
    uint64_t value = 0;

    //VulkanSemaphoreSubmitInfo(VulkanSemaphore& semaphore, VkPipelineStageFlags2 flags, uint64_t value) : 
    //  semaphore(semaphore), flags(flags), value(value) {}

    //VulkanSemaphoreSubmitInfo(VulkanSemaphore& semaphore, VkPipelineStageFlags2 flags) : 
    //  semaphore(semaphore), flags(flags) {}
  };

  class VulkanQueue {
    public:
      VulkanQueue(VulkanDevice& device, uint32_t queueFamily, uint32_t queueIndex); 

      void submit(std::vector<VulkanCommandBuffer>& commandBuffers, std::vector<VulkanSemaphoreSubmitInfo>* waitSemaphores, std::vector<VulkanSemaphoreSubmitInfo>* signalSemaphores, VulkanFence& fence);
      void submit(VulkanCommandBuffer& commandBuffer, std::vector<VulkanSemaphoreSubmitInfo>* waitSemaphores, std::vector<VulkanSemaphoreSubmitInfo>* signalSemaphores, VulkanFence& fence);
      void submit(VulkanCommandBuffer& commandBuffer, VulkanSemaphoreSubmitInfo* waitSemaphore, VulkanSemaphoreSubmitInfo* signalSemaphore, VulkanFence& fence);
      
      void present(VulkanSwapchain& swapchain, std::vector<uint32_t> imageIndicies, std::vector<VkSemaphore>& waitSemaphores);
      void present(VulkanSwapchain& swapchain, std::vector<uint32_t> imageIndicies, VkSemaphore& waitSemaphores);
    private:
      VkQueue queue;
  };
}
