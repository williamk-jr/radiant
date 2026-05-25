#include "radiant/core/render/Renderer.h"
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include <memory>
#include <vulkan/vulkan_core.h>

namespace Radiant {
  Renderer::Renderer(Window& window, bool debug) {
    this->instanceExtensions = this->getInstanceExtensions(window, debug); 
    this->instanceLayers = this->getInstanceLayers(debug);

    this->initVulkan(window, debug);
  }

  std::vector<const char*> Renderer::getInstanceExtensions(Window& window, bool debug) {
    std::vector<const char*> extensions = window.getSurfaceExtensions();
    extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    extensions.push_back(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);
    if (debug) extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    return extensions;
  }
  
  std::vector<const char*> Renderer::getInstanceLayers(bool debug) {
    std::vector<const char*> instanceLayers;
    if (debug) instanceLayers.push_back("VK_LAYER_KHRONOS_validation");
    return instanceLayers;
  }
  
  void Renderer::initVulkan(Window& window, bool debug) { 
    this->instance = std::make_unique<VulkanInstance>(window.getTitle(), this->instanceExtensions, this->instanceLayers);
    this->surface = std::make_unique<VulkanSurface>(*this->instance, window.getHandle());
    this->physicalDevice = std::make_unique<VulkanPhysicalDevice>(*this->instance, getPhysicalDeviceRequirements);

    VkPhysicalDeviceProperties2 deviceProperties = {};
    deviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    physicalDevice->getProperties(&deviceProperties);
    std::cout << "Selected Device: " << deviceProperties.properties.deviceName << "\n";

    std::vector<const char*> enabledDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    this->device = std::make_unique<VulkanDevice>(*this->physicalDevice, *this->surface, enabledDeviceExtensions); 
    this->graphicsQueue = std::make_unique<VulkanQueue>(*this->device, this->device->getGraphicsQueueFamily(), 0);
    this->presentQueue = std::make_unique<VulkanQueue>(*this->device, this->device->getPresentQueueFamily(), 0);
    this->swapchain = std::make_unique<VulkanSwapchain>(
        *this->physicalDevice, 
        *this->device, 
        *this->surface, 
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        0
    );

    this->memoryAllocator = std::make_unique<VulkanMemoryAllocator>(*instance, *physicalDevice, *device);
    this->commandPool = std::make_unique<VulkanCommandPool>(*device, device->getGraphicsQueueFamily());
    this->commandBuffers = this->commandPool->allocateCommandBuffers(1, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
  }

  bool Renderer::getPhysicalDeviceRequirements(VkPhysicalDevice& physicalDevice) {
    return true;
  }
}
