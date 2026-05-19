#pragma once
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "radiant/core/render/vulkan/VulkanInstance.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"

namespace Radiant {
  typedef bool (*VulkanPhysicalDeviceRequirements)(VkPhysicalDevice&);
  typedef bool (*VulkanQueueFamilyRequirements)(VkQueueFamilyProperties2&);

  class VulkanPhysicalDevice {
    public:
      VulkanPhysicalDevice(VulkanInstance& instance, VulkanPhysicalDeviceRequirements physicalDeviceRequirements);
      ~VulkanPhysicalDevice();

      VkPhysicalDevice& get();
      void getProperties(VkPhysicalDeviceProperties2* properties);
      std::vector<VkQueueFamilyProperties2> getQueueFamilyProperties();
      
      uint32_t getQueueFamily(std::vector<VkQueueFamilyProperties2> queueFamilies, VulkanQueueFamilyRequirements queueFamilyRequirements);
    private:
      VkPhysicalDevice physicalDevice;
  };
}
