#pragma once
#include <memory>
#include <set>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"
#include "radiant/core/render/vulkan/VulkanSurface.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"


namespace Radiant {
  class VulkanDevice {
    public:
      VulkanDevice(VulkanPhysicalDevice& physicalDevice, VulkanSurface& surface, std::vector<const char*>& extensions);
      ~VulkanDevice();

      VkDevice get();
      uint32_t getGraphicsQueueFamily();
      uint32_t getPresentQueueFamily();
      
      void waitIdle();

    private:
      VkDevice device;

      uint32_t graphicsQueueFamily;
      uint32_t presentQueueFamily;

      void createDevice(VulkanPhysicalDevice& physicalDevice, std::set<uint32_t>& queueFamilyIndicies, std::vector<const char*> extensions);
  };
}
