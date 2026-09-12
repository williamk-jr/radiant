#pragma once
#include <utility>
#include <vulkan/vulkan_core.h>

namespace Radiant {
	template <typename T>
	class VulkanResult {
		public:
			VulkanResult(VkResult result, T value) : result(result), value(std::move(value)) {}

			VkResult getResult() {
				return this->result;
			}

			T& getValue() & noexcept {
				return this->value;
			}

			T&& getValue() && noexcept {
				return std::move(this->value);
			}

			bool isSuccess() {
				return this->result == VK_SUCCESS;
			}

		private:
			VkResult result;
			T        value;
	};

	// Template specialization for void, simply representing a VkResult.
	template <>
	class VulkanResult<void> {
		public:
			VulkanResult(VkResult result) : result(result) {}

			VkResult getResult() {
				return this->result;
			}

			bool isSuccess() {
				return this->result == VK_SUCCESS;
			}

		private:
			VkResult result;
	};
} // namespace Radiant
