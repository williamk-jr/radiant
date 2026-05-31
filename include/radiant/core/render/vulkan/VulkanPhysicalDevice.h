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

  class VulkanPhysicalDevice {
    public:
      VulkanPhysicalDevice(VulkanInstance& instance, VulkanPhysicalDeviceRequirements physicalDeviceRequirements);
      VulkanPhysicalDevice(const VulkanPhysicalDevice&) = delete;
      VulkanPhysicalDevice& operator=(const VulkanPhysicalDevice&) = delete;

      VulkanPhysicalDevice(VulkanPhysicalDevice&&) noexcept;
      VulkanPhysicalDevice& operator=(VulkanPhysicalDevice&&) noexcept = default;
      ~VulkanPhysicalDevice();

      VkPhysicalDevice get();
      void getProperties(VkPhysicalDeviceProperties2* properties);
      std::vector<VkQueueFamilyProperties2> getQueueFamilyProperties();
      std::vector<VkSurfaceFormat2KHR> getSurfaceFormats(VulkanSurface& surface);
      bool queueFamilySupportsSurfaceKHR(VulkanSurface& surface, uint32_t queueFamily);
      
    private:
      VkPhysicalDevice physicalDevice;
  };
}
