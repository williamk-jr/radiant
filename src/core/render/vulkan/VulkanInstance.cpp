#include "radiant/core/render/vulkan/VulkanInstance.h"
#include "radiant/core/render/vulkan/VulkanUtil.h"

namespace Radiant {
  VulkanInstance::VulkanInstance(
      const std::string& applicationName, 
      const std::vector<const char*>& extensionNames,
      const std::vector<const char*>& layerNames
  ) { 
    VkApplicationInfo applicationInfo = {};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pApplicationName = applicationName.c_str();
    applicationInfo.apiVersion = VK_API_VERSION_1_4;

    VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo = {};
    debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;
    debugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    debugMessengerInfo.pfnUserCallback = debugCallback;
    debugMessengerInfo.pNext = nullptr;

    VkInstanceCreateInfo instanceInfo = {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.enabledLayerCount = layerNames.size();
    instanceInfo.ppEnabledLayerNames = layerNames.data();
    instanceInfo.enabledExtensionCount = extensionNames.size();
    instanceInfo.ppEnabledExtensionNames = extensionNames.data();
    instanceInfo.pApplicationInfo = &applicationInfo;
    instanceInfo.pNext = &debugMessengerInfo;

    Validation::verify(
      vkCreateInstance(&instanceInfo, nullptr, &this->instance)
    );
  }

  VulkanInstance::~VulkanInstance() {
    vkDestroyInstance(this->instance, nullptr);
  }

  VkInstance& VulkanInstance::get() {
    return this->instance;
  }
}
