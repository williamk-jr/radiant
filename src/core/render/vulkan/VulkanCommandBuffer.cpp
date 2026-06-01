#include "radiant/core/render/vulkan/VulkanCommandBuffer.h"
#include "radiant/core/render/vulkan/VulkanSwapchain.h"
#include <vulkan/vulkan_core.h>

namespace Radiant {

  VulkanCommandBuffer::VulkanCommandBuffer(VkCommandBuffer commandBuffer) : commandBuffer(commandBuffer) {
    
  }

  VulkanCommandBuffer::VulkanCommandBuffer(VulkanCommandBuffer&& other) noexcept :
    commandBuffer(other.commandBuffer) {
    other.commandBuffer = nullptr;
  }

  VkCommandBuffer VulkanCommandBuffer::get() {
    return this->commandBuffer;
  }

  void VulkanCommandBuffer::begin(VkCommandBufferUsageFlags flags) {
    //VkCommandBufferInheritanceInfo commandBufferInheritanceInfo{};

    VkCommandBufferBeginInfo commandBufferBeginInfo{};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.flags = flags;
    commandBufferBeginInfo.pInheritanceInfo = nullptr; // TODO research secondary vs. primary command buffers.
    
    Validation::verify(
      vkBeginCommandBuffer(this->commandBuffer, &commandBufferBeginInfo)
    );
  }
  
  void VulkanCommandBuffer::pipelineMemoryBarrier(std::vector<VkMemoryBarrier2>& memoryBarriers, VkDependencyFlags dependencyFlags) {
    VkDependencyInfo dependencyInfo{};
    dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependencyInfo.memoryBarrierCount = memoryBarriers.size();
    dependencyInfo.pMemoryBarriers = memoryBarriers.data();
    dependencyInfo.dependencyFlags = dependencyFlags;

    vkCmdPipelineBarrier2(this->commandBuffer, &dependencyInfo);
  }

  void VulkanCommandBuffer::pipelineImageMemoryBarrier(std::vector<VkImageMemoryBarrier2>& memoryBarriers, VkDependencyFlags dependencyFlags) {
    VkDependencyInfo dependencyInfo{};
    dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependencyInfo.imageMemoryBarrierCount = memoryBarriers.size();
    dependencyInfo.pImageMemoryBarriers = memoryBarriers.data();
    dependencyInfo.dependencyFlags = dependencyFlags;

    vkCmdPipelineBarrier2(this->commandBuffer, &dependencyInfo);
  }

  void VulkanCommandBuffer::pipelineBufferMemoryBarrier(std::vector<VkBufferMemoryBarrier2>& memoryBarriers, VkDependencyFlags dependencyFlags) {
    VkDependencyInfo dependencyInfo{};
    dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependencyInfo.bufferMemoryBarrierCount = memoryBarriers.size();
    dependencyInfo.pBufferMemoryBarriers = memoryBarriers.data();
    dependencyInfo.dependencyFlags = dependencyFlags;

    vkCmdPipelineBarrier2(this->commandBuffer, &dependencyInfo);
  }
  
  void VulkanCommandBuffer::clearColor(VulkanImage& image, VkClearColorValue& color) {
    VkImageSubresourceRange imageSubresourceRange{};
    imageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageSubresourceRange.layerCount = 1;
    imageSubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
    //vkTransitionImageLayout(VkDevice device, uint32_t transitionCount, const VkHostImageLayoutTransitionInfo *pTransitions)
    vkCmdClearColorImage(this->commandBuffer, image.get(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &color, 1, &imageSubresourceRange);
  }
  
  void VulkanCommandBuffer::beginRendering(std::vector<VkRenderingAttachmentInfo>* colorAttachments, VkRenderingAttachmentInfo* depthAttachment, VkRenderingAttachmentInfo* stencilAttachment, VkRect2D renderArea, VkRenderingFlags renderingFlags) {
    VkRenderingInfo renderingInfo{};
    renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    renderingInfo.flags = renderingFlags;
    renderingInfo.layerCount = 1;
    renderingInfo.renderArea = renderArea;
    renderingInfo.colorAttachmentCount = colorAttachments->size();
    renderingInfo.pColorAttachments = colorAttachments->data();
    renderingInfo.pDepthAttachment = depthAttachment;
    renderingInfo.pStencilAttachment = stencilAttachment;

    vkCmdBeginRendering(this->commandBuffer, &renderingInfo);
  }

  void VulkanCommandBuffer::clearAttachments(std::vector<VkClearAttachment> clearAttachments, std::vector<VkClearRect> clearAreas) {
    vkCmdClearAttachments(
        this->commandBuffer, 
        clearAttachments.size(), clearAttachments.data(), 
        clearAreas.size(), clearAreas.data()
    );
  }
  
  void VulkanCommandBuffer::clearAttachment(VulkanImage& image, VkClearAttachment clearAttachment) {
    VkExtent3D imageExtent = image.getExtent();
    VkRect2D imageSize{};
    imageSize.extent.width = imageExtent.width;
    imageSize.extent.height = imageExtent.height;

    VkClearRect clearArea{};
    clearArea.rect = imageSize;
    clearArea.baseArrayLayer = 0;
    clearArea.layerCount = 1;

    this->clearAttachments({clearAttachment}, {clearArea});
  }

  void VulkanCommandBuffer::endRendering() {
    vkCmdEndRendering(this->commandBuffer);
  }
  
  //void VulkanCommandBuffer::beginRendering(VkRenderingFlags renderingFlags) {
  //  VkImageView imageView;
  //  VkImageViewCreateInfo imageViewInfo{};
  //  imageViewInfo.image = nullptr;
  //  vkCmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue *pColor, uint32_t rangeCount, const VkImageSubresourceRange *pRanges)

  //  vkCreateImageView(VkDevice device, const VkImageViewCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkImageView *pView)
  //  
  //  VkRenderingAttachmentInfo renderingAttachmentInfo{};
  //  renderingAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
  //  renderingAttachmentInfo.imageView = VK_IMAGE_VIEW_TYPE_2D;
  //  renderingAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  //  renderingAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  //  renderingAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

  //  VkRenderingInfo renderingInfo{};
  //  renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
  //  renderingInfo.flags = renderingFlags;
  //  renderingInfo.layerCount = 1;

  //  vkCmdBeginRendering(this->commandBuffer, &renderingInfo);
  //}
  
  void VulkanCommandBuffer::end() {
    Validation::verify(
      vkEndCommandBuffer(this->commandBuffer)
    );
  }

  void VulkanCommandBuffer::reset(bool recycleResources) {
    vkResetCommandBuffer(this->commandBuffer, recycleResources ? VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT : 0);
  }
}
