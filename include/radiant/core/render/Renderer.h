#pragma once
#include <memory>
#include <vector>

#include "radiant/core/render/Window.h"
#include "radiant/core/render/vulkan/VulkanBinarySemaphore.h"
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanFence.h"
#include "radiant/core/render/vulkan/VulkanInstance.h"
#include "radiant/core/render/vulkan/VulkanMemoryAllocator.h"
#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"
#include "radiant/core/render/vulkan/VulkanQueue.h"
#include "radiant/core/render/vulkan/VulkanSurface.h"
#include "radiant/core/render/vulkan/VulkanSwapchain.h"
#include "radiant/core/render/vulkan/VulkanCommandPool.h"
#include "radiant/core/render/vulkan/VulkanImage.h"

namespace Radiant {
  class Renderer {
    public:
      Renderer(Window& window, bool debug);
      void waitIdle();

      uint32_t beginFrame();
      void endFrame();
      void renderLoop();
    private:
      std::vector<const char*> instanceExtensions;
      std::vector<const char*> instanceLayers;

      std::unique_ptr<VulkanInstance> instance;
      std::unique_ptr<VulkanSurface> surface;
      std::unique_ptr<VulkanPhysicalDevice> physicalDevice;

      std::unique_ptr<VulkanDevice> device;
      std::unique_ptr<VulkanMemoryAllocator> memoryAllocator;
      std::unique_ptr<VulkanQueue> graphicsQueue;
      std::unique_ptr<VulkanQueue> presentQueue;

      std::unique_ptr<VulkanSwapchain> swapchain;

      std::unique_ptr<VulkanCommandPool> commandPool;
      std::vector<VulkanCommandBuffer> commandBuffers;
      
      std::vector<VulkanFence> fences;
      std::vector<VulkanBinarySemaphore> imageReadySemaphores;
      std::vector<VulkanBinarySemaphore> frameFinishedSemaphores;
      int currentFrame = 0;
      bool isRendering = false;

      void initVulkan(Window& window, bool debug);
      std::vector<const char*> getInstanceExtensions(Window& window, bool debug);
      std::vector<const char*> getInstanceLayers(bool debug);
      static bool getPhysicalDeviceRequirements(VkPhysicalDevice& physicalDevice);
  };
}
