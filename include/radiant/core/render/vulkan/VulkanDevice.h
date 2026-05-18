#pragma once
#include <set>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"
#include "radiant/core/render/vulkan/VulkanSurface.h"

namespace Radiant {
  class VulkanDevice {
    public:
      VulkanDevice(VulkanPhysicalDevice& physicalDevice, VulkanSurface& surface, std::vector<const char*> extensions);
      ~VulkanDevice();

    private:
      VkDevice device;
      VkQueue graphicsQueue;
      VkQueue presentQueue;

      void createDevice(VulkanPhysicalDevice& physicalDevice, std::set<uint32_t> queueFamilyIndicies, std::vector<const char*> extensions);
  };
}
