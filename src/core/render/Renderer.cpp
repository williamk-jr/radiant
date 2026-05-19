#include "radiant/core/render/Renderer.h"

namespace Radiant {
  Renderer::Renderer(Window& window, bool debug) {
    this->instanceExtensions = this->getInstanceExtensions(window, debug); 
    this->instanceLayers = this->getInstanceLayers(debug);

    this->initVulkan(window, debug);
  }

  std::vector<const char*> Renderer::getInstanceExtensions(Window& window, bool debug) {
    std::vector<const char*> extensions = window.getSurfaceExtensions();
    extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
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

    this->memoryAllocator = std::make_unique<VulkanMemoryAllocator>(*instance, *physicalDevice, *device);
    this->commandPool = std::make_unique<VulkanCommandPool>(*device, device->getGraphicsQueueFamily());
  }

  bool Renderer::getPhysicalDeviceRequirements(VkPhysicalDevice& physicalDevice) {
    return true;
  }
}
