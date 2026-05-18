#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <vulkan/vulkan.h>

namespace Radiant {
  class VulkanInstance {
    public:
      VulkanInstance(
        const std::string& applicationName,
        const std::vector<const char*>& extensionNames,
        const std::vector<const char*>& layerNames
      );
      ~VulkanInstance();
      
      VkInstance& get();
      
    private:
      VkInstance instance;

      static VkBool32 debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageTypes,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData
      ) {
        if (messageSeverity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
          std::cout << "[INFO] " << "[" << pCallbackData->pMessageIdName << "] " << pCallbackData->pMessage << "\n";
        } else if (messageSeverity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
          std::cout << "[WARN]" << "[" << pCallbackData->pMessageIdName << "] " << pCallbackData->pMessage << "\n";
        } else if (messageSeverity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
          std::cerr << "[ERROR]" << "[" << pCallbackData->pMessageIdName << "] " << pCallbackData->pMessage << "\n";
        }

        return VK_FALSE;
      }
  };
}
