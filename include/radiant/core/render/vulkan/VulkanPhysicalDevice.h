#pragma once
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "radiant/core/render/vulkan/VulkanInstance.h"
#include "radiant/core/render/vulkan/VulkanSurface.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"

namespace Radiant {
  typedef bool (*VulkanPhysicalDeviceRequirements)(VkPhysicalDevice&);

  /* VulkanPhysicalDevice
   *
   * Wrapper for VkPhysicalDevice.
   * Represents a valid device for graphics processing.
   */
  class VulkanPhysicalDevice {
    public:
      /*
       * @param VulkanInstance A valid vulkan instance.
       * @param VulkanPhysicalDeviceRequirements A callback containing logic for selecting a valid physical device. 
       */
      VulkanPhysicalDevice(VulkanInstance& instance, VulkanPhysicalDeviceRequirements physicalDeviceRequirements);
      VulkanPhysicalDevice(const VulkanPhysicalDevice&) = delete;
      VulkanPhysicalDevice& operator=(const VulkanPhysicalDevice&) = delete;

      VulkanPhysicalDevice(VulkanPhysicalDevice&&) noexcept;
      VulkanPhysicalDevice& operator=(VulkanPhysicalDevice&&) noexcept = default;
      ~VulkanPhysicalDevice();

      /* @return Returns a raw VkPhysicalDevice. */
      VkPhysicalDevice get();

      /*
       * @return The properties of this physical device.
       */
      void getProperties(VkPhysicalDeviceProperties2* properties);

      /*
       * @return Returns a vector of properties for each queue family.
       */
      std::vector<VkQueueFamilyProperties2> getQueueFamilyProperties();

      /*
       * @param VulkanSurface A reference to a valid vulkan surface.
       * @return A vector of avaliable surface formats.
       */
      std::vector<VkSurfaceFormat2KHR> getSurfaceFormats(VulkanSurface& surface);

      /*
       * @param VulkanSurface A reference to a valid vulkan surface.
       * @param uint32_t The index of a queue family as provided by getQueueFamilyProperties().
       * @return Whether the provided queue amily supports the provided surface.
       */
      bool queueFamilySupportsSurfaceKHR(VulkanSurface& surface, uint32_t queueFamily);
      
    private:
      VkPhysicalDevice physicalDevice;
  };
}
