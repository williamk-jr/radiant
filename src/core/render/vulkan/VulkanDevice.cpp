#include "radiant/core/render/vulkan/VulkanDevice.h"

namespace Radiant {
  VulkanDevice::VulkanDevice(VulkanPhysicalDevice& physicalDevice, VulkanSurface& surface, std::vector<const char*> extensions) {
    std::vector<VkQueueFamilyProperties2> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

    uint32_t graphicsQueueFamily = -1;
    uint32_t presentQueueFamily = -1;
    // TODO Generalize queue creation.

    for (int i = 0; i < queueFamilyProperties.size(); i++) {
      VkQueueFamilyProperties& properties = queueFamilyProperties[i].queueFamilyProperties;
      if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        graphicsQueueFamily = i;
      }

      if (physicalDevice.queueFamilySupportsSurfaceKHR(surface, i)) {
        presentQueueFamily = i;
      }

      if (graphicsQueueFamily > -1 && presentQueueFamily > -1) {
        break;
      }
    }

    this->graphicsQueueFamily = graphicsQueueFamily;
    this->presentQueueFamily = presentQueueFamily;
    this->createDevice(physicalDevice, {graphicsQueueFamily, presentQueueFamily}, extensions);

  }

  VulkanDevice::~VulkanDevice() {
    vkDestroyDevice(this->device, nullptr);
  }

  VkDevice& VulkanDevice::get() {
    return this->device;
  }

  uint32_t VulkanDevice::getGraphicsQueueFamily() {
    return this->graphicsQueueFamily;
  }

  uint32_t VulkanDevice::getPresentQueueFamily() {
    return this->presentQueueFamily;
  }

  void VulkanDevice::createDevice(VulkanPhysicalDevice& physicalDevice, std::set<uint32_t> queueFamilyIndicies, std::vector<const char*> extensions) {
    float priority = 1.0;
    std::vector<VkDeviceQueueCreateInfo> deviceQueueInfos;
    for (uint32_t queueFamilyIndex : queueFamilyIndicies) {
      VkDeviceQueueCreateInfo deviceQueueInfo{};
      deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      deviceQueueInfo.queueCount = 1;
      deviceQueueInfo.queueFamilyIndex = queueFamilyIndex;
      deviceQueueInfo.pQueuePriorities = &priority;
      deviceQueueInfos.push_back(deviceQueueInfo);
    }
    
    VkDeviceCreateInfo deviceInfo{};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = deviceQueueInfos.size();
    deviceInfo.pQueueCreateInfos = deviceQueueInfos.data();
    deviceInfo.enabledExtensionCount = extensions.size();
    deviceInfo.ppEnabledExtensionNames = extensions.data();

    Validation::verify(
      vkCreateDevice(physicalDevice.get(), &deviceInfo, nullptr, &this->device)
    );
  }
}
