#pragma once

#include "radiant/core/render/vulkan/VulkanDevice.h"
#include <vulkan/vulkan_core.h>
namespace Radiant {
  /* VulkanFence
   * 
   * Wrapper for VkFence.
   */
  class VulkanFence {
    public:
      /*
       * @param VulkanDevice& A reference to a valid vulkan device.
       * @param VkFenceCreateFlags Create flags.
       */
      VulkanFence(VulkanDevice& device, VkFenceCreateFlags flags);
      VulkanFence(const VulkanFence&) = delete;
      VulkanFence& operator=(const VulkanFence&) = delete;

      VulkanFence(VulkanFence&&) noexcept;
      VulkanFence& operator=(VulkanFence&&) noexcept = default;
      ~VulkanFence();

      /*
       * @return A raw VkFence.
       */
      VkFence get();

      /*
       * Resets fence state.
       */
      void reset();

      /*
       * @param uint32_t Maximum amount of time to wait on this fence.
       */
      void wait(uint32_t timeout);

      /*
       * Checks if this fence is signaled
       *
       * @return Whether fence is signaled.
       */
      bool isSignaled();

      /*
       * Waits until at least one of the provided fences are signaled.
       *
       * @param VulkanDevice& A reference to a valid vulkan device.
       * @param std::vector<VulkanFence>& A reference to a vector of vulkan fences.
       * @param bool Whether to wait until all fences are signaled.
       * @param uint32_t Maximum amount of time to wait on provided fences.
       */
      static void waitForFences(VulkanDevice& device, std::vector<VulkanFence>& fences, bool waitAll, uint64_t timeout);
    private:
      VkFence fence;
      VkDevice device;
  };
}
