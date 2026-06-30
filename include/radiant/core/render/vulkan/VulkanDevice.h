#pragma once
#include <memory>
#include <set>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"
#include "radiant/core/render/vulkan/VulkanSurface.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"


namespace Radiant {
  /* VulkanDevice
   *
   * A wrapper for VkDevice.
   * Represents a logical device that handles dispatching commands to the physical device.
   */
  class VulkanDevice {
    public:
      VulkanDevice(VulkanPhysicalDevice& physicalDevice, VulkanSurface& surface, std::vector<const char*>& extensions);
      VulkanDevice(const VulkanDevice&) = delete;
      VulkanDevice& operator=(const VulkanDevice&) = delete;

      VulkanDevice(VulkanDevice&&) noexcept;
      VulkanDevice& operator=(VulkanDevice&&) noexcept = default;
      ~VulkanDevice();

      /*
       * @return A raw VkDevice.
       */
      VkDevice get();

      /*
       * @return Index to a queue with graphics capabilities.
       */
      uint32_t getGraphicsQueueFamily();

      /*
       * @return Index to a queue with present capabilities.
       */
      uint32_t getPresentQueueFamily();
      
      /*
       * Waits until this device is no longer processing any work.
       */
      void waitIdle();

    private:
      VkDevice device;

      uint32_t graphicsQueueFamily;
      uint32_t presentQueueFamily;

      void createDevice(VulkanPhysicalDevice& physicalDevice, std::set<uint32_t>& queueFamilyIndicies, std::vector<const char*> extensions);
  };
}
