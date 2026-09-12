#include "radiant/core/render/vulkan/VulkanSurface.h"

#include "radiant/core/render/vulkan/VulkanUtil.h"

namespace Radiant {
#ifdef HAS_GLFW
	VulkanSurface::VulkanSurface(VulkanInstance& instance, GLFWwindow* window) : instance(instance.get()) {
		VkResult result = glfwCreateWindowSurface(instance.get(), window, nullptr, &this->surface);
		VulkanUtil::validate("VulkanSurface Initialization Error. Failed to create surface.", result);
	}
#endif

#ifdef HAS_SDL3
	SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface)
#endif

	    VulkanSurface::VulkanSurface(VulkanSurface&& other) noexcept
	    : surface(other.surface), instance(other.instance) {
		other.surface = nullptr;
	}

	VulkanSurface::~VulkanSurface() {
		vkDestroySurfaceKHR(this->instance, this->surface, nullptr);
	}

	VkSurfaceKHR VulkanSurface::get() {
		return this->surface;
	}
} // namespace Radiant
