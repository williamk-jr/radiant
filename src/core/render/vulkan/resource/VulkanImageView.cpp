#include "radiant/core/render/vulkan/resource/VulkanImageView.h"

#include "radiant/core/render/vulkan/VulkanUtil.h"

namespace Radiant {
	VulkanImageView::VulkanImageView(VulkanDevice&           device,
	                                 VulkanImage&            image,
	                                 VkFormat                format,
	                                 VkImageSubresourceRange subresourceRange)
	    : VulkanImageView(device, image, format, {}, subresourceRange, 0) {}

	VulkanImageView::VulkanImageView(VulkanDevice&           device,
	                                 VulkanImage&            image,
	                                 VkFormat                format,
	                                 VkComponentMapping      componentMapping,
	                                 VkImageSubresourceRange subresourceRange,
	                                 VkImageViewCreateFlags  flags)
	    : device(device.get()) {
		VkImageViewCreateInfo imageViewInfo{};
		imageViewInfo.sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image            = image.get();
		imageViewInfo.viewType         = VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.format           = format;
		imageViewInfo.components       = componentMapping;
		imageViewInfo.subresourceRange = subresourceRange;
		imageViewInfo.flags            = flags;

		VkResult result = vkCreateImageView(device.get(), &imageViewInfo, nullptr, &this->imageView);
		VulkanUtil::validate("VulkanImageView Initialization Error. Failed to create image view.", result);

		this->extent = image.getExtent();
	}

	VulkanImageView::VulkanImageView(VulkanImageView&& other) noexcept
	    : imageView(other.imageView), extent(other.extent), device(other.device) {
		other.imageView = nullptr;
	}

	VulkanImageView::~VulkanImageView() {
		vkDestroyImageView(this->device, this->imageView, nullptr);
	}

	VkImageView VulkanImageView::get() {
		return this->imageView;
	}

	VkExtent3D VulkanImageView::getExtent() {
		return this->extent;
	}
} // namespace Radiant
