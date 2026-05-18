#pragma once
#include "radiant/util/logger/Logger.h"
#include "radiant/util/logger/MessageStyle.h"
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
          Logger::info(pCallbackData->pMessage, {
            {pCallbackData->pMessageIdName, MessageStyle::WHITE},
            LogPrefixes::time(MessageStyle::WHITE)
          });
        } else if (messageSeverity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
          Logger::warn(pCallbackData->pMessage, {
            {pCallbackData->pMessageIdName, MessageStyle::WHITE},
            LogPrefixes::time(MessageStyle::WHITE)
          });
        } else if (messageSeverity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
          Logger::error(pCallbackData->pMessage, {
            {pCallbackData->pMessageIdName, MessageStyle::WHITE},
            LogPrefixes::time(MessageStyle::WHITE)
          });
        }
        return VK_FALSE;
      }
  };
}
