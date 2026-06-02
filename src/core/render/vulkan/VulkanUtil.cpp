#include "radiant/core/render/vulkan/VulkanUtil.h"

namespace Radiant {
  namespace Validation {
    void verify(VkResult result) {
      if (result != VK_SUCCESS) {
        Logger::fatal("Something went wrong. Vulkan Error Code: " + std::to_string(result), {
          {"VULKAN", MessageStyle::WHITE},
          LogPrefixes::time(MessageStyle::WHITE)
        });
      }
    }
  }
}
