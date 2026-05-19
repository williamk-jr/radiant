#pragma once
#include "radiant/util/logger/Logger.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {
  namespace Validation {
    void verify(VkResult result);
  }
}
