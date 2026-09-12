#pragma once
#include <string>
#include <vulkan/vulkan_core.h>

namespace Radiant {
	namespace VulkanUtil {
		void validate(std::string message, VkResult result);
	}
} // namespace Radiant
