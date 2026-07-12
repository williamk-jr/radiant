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
  };

  /* VulkanQueue
   * 
   * A wrapper for VkQueue
   * Represents a queue for which all comand buffer commands are submitted to for execution by the physical device.
   */
  class VulkanQueue {
    public:
      /*
       * @param VulkanDevice& A reference to a valid vulkan device.
       * @param uint32_t Index of the queue family this queue will be created from.
       * @param uint32_t Index at which this queue will be created.
       */
      VulkanQueue(VulkanDevice& device, uint32_t queueFamily, uint32_t queueIndex); 
      VulkanQueue(const VulkanQueue&) = delete;
      VulkanQueue& operator=(const VulkanQueue&) = delete;

      VulkanQueue(VulkanQueue&&) noexcept;
      VulkanQueue& operator=(VulkanQueue&&) noexcept = default;

      /*
       * Submits command buffers for their commands to be executed.
       *
       * @param std::vector<VulkanCommandBuffer*>& A reference to a vector of valid command buffer pointers.
       * @param std::vector<VulkanSemaphoreSubmitInfo>* A pointer to a vector of semaphores to be waited on.
       * @param std::vector<VulkanSemaphoreSubmitInfo>* A pointer to a vector of semaphores to signal once execution of command buffers is complete.
       * @param VulkanFence& A reference to a valid vulkan fence to wait on before work can be submitted.
       */
      void submit(std::vector<VulkanCommandBuffer*>& commandBuffers, std::vector<VulkanSemaphoreSubmitInfo>& waitSemaphores, std::vector<VulkanSemaphoreSubmitInfo>& signalSemaphores, VulkanFence& fence);

      /*
       * Submits command buffers for their commands to be executed.
       *
       * @param VulkanCommandBuffer& A reference to a valid vulkan command buffer.
       * @param std::vector<VulkanSemaphoreSubmitInfo> A reference to a vector of semaphores to be waited on.
       * @param std::vector<VulkanSemaphoreSubmitInfo> A reference to a vector of semaphores to signal once execution of command buffers is complete.
       * @param VulkanFence& A reference to a valid vulkan fence to wait on before work can be submitted.
       */
      void submit(VulkanCommandBuffer& commandBuffer, std::vector<VulkanSemaphoreSubmitInfo>& waitSemaphores, std::vector<VulkanSemaphoreSubmitInfo>& signalSemaphores, VulkanFence& fence);

      /*
       * Submits command buffers for their commands to be executed.
       *
       * @param VulkanCommandBuffer& A reference to a valid vulkan command buffer.
       * @param VulkanSemaphoreSubmitInfo* A pointer to a semaphore to be waited on.
       * @param VulkanSemaphoreSubmitInfo A pointer to a semaphore to signal once execution of the command buffer is complete.
       * @param VulkanFence& A reference to a valid vulkan fence to wait on before work can be submitted.
       */
      void submit(VulkanCommandBuffer& commandBuffer, VulkanSemaphoreSubmitInfo* waitSemaphore, VulkanSemaphoreSubmitInfo* signalSemaphore, VulkanFence& fence);
      
      /* Presents an image in the swapchain to a surface.
       * 
       * @param VulkanSwapchain& A reference to a valid vulkan swapchain.
       * @param std::vector<uint32_t> A vector of image indicies to present from swapchain.
       * @param std::vector<VulkanSemaphore*>& A reference to a vector of valid semaphore pointers to wait on before presenting.
       */
      void present(VulkanSwapchain& swapchain, std::vector<uint32_t> imageIndicies, std::vector<VulkanSemaphore*>& waitSemaphores);

      /* Presents an image in the swapchain to a surface.
       * 
       * @param VulkanSwapchain& A reference to a valid vulkan swapchain.
       * @param std::vector<uint32_t> A vector of image indicies to present from swapchain.
       * @param VulkanSemaphore& A reference to a valid semaphore to wait on before presenting.
       */
      void present(VulkanSwapchain& swapchain, std::vector<uint32_t> imageIndicies, VulkanSemaphore& waitSemaphores);
    private:
      VkQueue queue;
  };
}
