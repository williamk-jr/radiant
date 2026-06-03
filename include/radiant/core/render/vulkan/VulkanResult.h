#pragma once
#include <vulkan/vulkan_core.h>

namespace Radiant {
  template<typename T>
  struct VulkanResult {
    VkResult result;
    T value;
    
    //operator T() const {
    //  return value;
    //}
    //operator VkResult() const {
    //  return result;
    //}
  };
}
