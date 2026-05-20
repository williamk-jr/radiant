#pragma once
#include <memory>
#include <vector>

#include "radiant/core/render/Window.h"
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanInstance.h"
#include "radiant/core/render/vulkan/VulkanMemoryAllocator.h"
#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"
#include "radiant/core/render/vulkan/VulkanSurface.h"
#include "radiant/core/render/vulkan/VulkanCommandPool.h"

namespace Radiant {
  class Renderer {
    public:
      Renderer(Window& window, bool debug);
    private:
      std::vector<const char*> instanceExtensions;
      std::vector<const char*> instanceLayers;

      std::unique_ptr<VulkanInstance> instance;
      std::unique_ptr<VulkanSurface> surface;
      std::unique_ptr<VulkanPhysicalDevice> physicalDevice;
      std::unique_ptr<VulkanDevice> device;

      std::unique_ptr<VulkanMemoryAllocator> memoryAllocator;
      std::unique_ptr<VulkanCommandPool> commandPool;
      std::vector<VulkanCommandBuffer> commandBuffers;

      void initVulkan(Window& window, bool debug);
      std::vector<const char*> getInstanceExtensions(Window& window, bool debug);
      std::vector<const char*> getInstanceLayers(bool debug);
      static bool getPhysicalDeviceRequirements(VkPhysicalDevice& physicalDevice);
  };
}
