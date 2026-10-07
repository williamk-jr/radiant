#pragma once

#include <iostream>
#include <span>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace Radiant {
	using VulkanLogCallback = void (*)(std::string);

	/* VulkanInstance
	 *
	 *  Wrapper class for VkInstance.
	 */
	class VulkanInstance {
		public:
			static VulkanLogCallback LOG_ERROR;
			static VulkanLogCallback LOG_WARNING;
			static VulkanLogCallback LOG_INFO;

			VulkanInstance(const std::string& applicationName);

			VulkanInstance(const std::string&           applicationName,
			               std::span<const char* const> extensionNames,
			               std::span<const char* const> layerNames);

			VulkanInstance(const VulkanInstance&)            = delete;
			VulkanInstance& operator=(const VulkanInstance&) = delete;

			VulkanInstance(VulkanInstance&&) noexcept;
			VulkanInstance& operator=(VulkanInstance&&) noexcept;
			~VulkanInstance();

			/*
			 * @return returns a raw VkInstance
			 */
			VkInstance get();

		private:
			VkInstance instance = VK_NULL_HANDLE;

			static VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
			                              VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
			                              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			                              void*                                       pUserData) {
				if (messageSeverity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
					VulkanInstance::LOG_INFO(pCallbackData->pMessage);
				} else if (messageSeverity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
					VulkanInstance::LOG_INFO(pCallbackData->pMessage);
				} else if (messageSeverity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
					VulkanInstance::LOG_WARNING(pCallbackData->pMessage);
				} else if (messageSeverity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
					VulkanInstance::LOG_ERROR(pCallbackData->pMessage);
				}
				return VK_FALSE;
			}
	};
} // namespace Radiant
