#include "radiant/core/render/vulkan/VulkanUtil.h"

#include "radiant/util/logger/Logger.h"

#include <string>

namespace Radiant {
	namespace VulkanUtil {
		void validate(std::string message, VkResult result) {
			if (result != VK_SUCCESS) {
				Logger::fatal(message + " Error Code: " + std::to_string(result),
				              {{"VULKAN", MessageStyle::WHITE}, LogPrefixes::time(MessageStyle::WHITE)});
			}
		}
	} // namespace VulkanUtil
} // namespace Radiant
