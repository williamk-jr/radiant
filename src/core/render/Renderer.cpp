#include "radiant/core/render/Renderer.h"

namespace Radiant {
  Renderer::Renderer(Window& window, bool debug) {
    this->instanceExtensions = this->getInstanceExtensions(window, debug); 
    this->instanceLayers = this->getInstanceLayers(debug);

    this->initVulkan(window, debug);
  }

  void Renderer::waitIdle() {
    this->device->waitIdle();
  } 

  void Renderer::beginFrame() {
    this->fences[currentFrame].wait(UINT32_MAX);

    uint32_t imageIndex = this->swapchain->acquireNextImage(&this->imageReadySemaphores[currentFrame], UINT64_MAX);
    VulkanImage& currentImage = this->swapchain->getImage(imageIndex);
    VulkanImageView& currentImageView = this->swapchain->getImageView(imageIndex);

    VkImageSubresourceRange subresourceRange{};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;

    this->fences[currentFrame].reset();
    this->commandBuffers[currentFrame].reset(false);
    this->commandBuffers[currentFrame].begin(0);

    // Transition image layout to transfer dst optimal.
    VkImageMemoryBarrier2 imageMemoryBarrier{};
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    imageMemoryBarrier.srcStageMask = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
    imageMemoryBarrier.srcAccessMask = 0;
    imageMemoryBarrier.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT | VK_ACCESS_2_MEMORY_WRITE_BIT;
    imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
    imageMemoryBarrier.image = currentImage.get();
    imageMemoryBarrier.subresourceRange = subresourceRange;

    std::vector<VkImageMemoryBarrier2> imageMemoryBarriers{imageMemoryBarrier};
    this->commandBuffers[currentFrame].pipelineImageMemoryBarrier(imageMemoryBarriers, 0);

    this->context = std::make_unique<RenderContext>(
      currentImageView,
      imageIndex
    );
  }

  void Renderer::beginRendering(Color clearColor) {
    float* rawColor = clearColor.raw();

    std::vector<VkRenderingAttachmentInfo> colorAttachment(1);
    colorAttachment[0].sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    colorAttachment[0].imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
    colorAttachment[0].imageView = this->context->imageView.get(); 
    colorAttachment[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment[0].clearValue = {{rawColor[0], rawColor[1], rawColor[2], rawColor[3]}};

    VulkanImage& currentImage = this->swapchain->getImage(this->context->imageIndex);
    VkExtent3D imageExtent = currentImage.getExtent(); // TODO remove getExtent, as changing swapchain extent will cause image extent to be out of date.

    VkRect2D renderArea{};
    renderArea.extent = {imageExtent.width, imageExtent.height};

    this->commandBuffers[currentFrame].beginRendering(&colorAttachment, nullptr, nullptr, renderArea, 0);
  }

  void Renderer::clear(Color color) {
    VulkanImage& currentImage = this->swapchain->getImage(this->context->imageIndex);
    VkExtent3D imageExtent = currentImage.getExtent();
    this->clear(color, {{0,0},{imageExtent.width, imageExtent.height}});
  }

  void Renderer::clear(Color color, VkRect2D clearArea) {
    float* rawColor = color.raw();

    VkClearAttachment clearAttachment{};
    clearAttachment.clearValue = {{rawColor[0], rawColor[1], rawColor[2], rawColor[3]}};
    clearAttachment.colorAttachment = 0;
    clearAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    VkClearRect clearRect{};
    clearRect.rect = clearArea;
    clearRect.baseArrayLayer = 0;
    clearRect.layerCount = 1;
  
    this->commandBuffers[currentFrame].clearAttachments({clearAttachment}, {clearRect});
  }

  void Renderer::endRendering() {
    this->commandBuffers[currentFrame].endRendering();
  }

  void Renderer::endFrame() {
    VkImageSubresourceRange subresourceRange{};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;

    VkImageMemoryBarrier2 presentImageMemoryBarrier{};
    presentImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    presentImageMemoryBarrier.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    presentImageMemoryBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
    presentImageMemoryBarrier.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
    presentImageMemoryBarrier.dstAccessMask = 0;
    presentImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
    presentImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    presentImageMemoryBarrier.image = this->swapchain->getImage(this->context->imageIndex).get();
    presentImageMemoryBarrier.subresourceRange = subresourceRange;

    std::vector<VkImageMemoryBarrier2> presentImageMemoryBarriers{presentImageMemoryBarrier};
    this->commandBuffers[currentFrame].pipelineImageMemoryBarrier(presentImageMemoryBarriers, 0);
    this->commandBuffers[currentFrame].end();
  }

  void Renderer::submit() {
    VulkanSemaphoreSubmitInfo imageReadySemaphoreSubmitInfo{};
    imageReadySemaphoreSubmitInfo.semaphore = &this->imageReadySemaphores[currentFrame];
    imageReadySemaphoreSubmitInfo.flags = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;

    VulkanSemaphoreSubmitInfo frameFinishedSemaphoreSubmitInfo{};
    frameFinishedSemaphoreSubmitInfo.semaphore = &this->frameFinishedSemaphores[currentFrame];
    

    this->graphicsQueue->submit(
        this->commandBuffers[currentFrame], 
        &imageReadySemaphoreSubmitInfo, 
        &frameFinishedSemaphoreSubmitInfo, 
        this->fences[currentFrame]
    );
  }

  void Renderer::present() {
    this->presentQueue->present(*this->swapchain, {this->context->imageIndex}, this->frameFinishedSemaphores[currentFrame]);
    this->currentFrame = (currentFrame+1)%swapchain->getImageCount();
  }

  std::vector<const char*> Renderer::getInstanceExtensions(Window& window, bool debug) {
    std::vector<const char*> extensions = window.getSurfaceExtensions();
    extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    extensions.push_back(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);
    if (debug) extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    return extensions;
  }
  
  std::vector<const char*> Renderer::getInstanceLayers(bool debug) {
    std::vector<const char*> instanceLayers;
    if (debug) instanceLayers.push_back("VK_LAYER_KHRONOS_validation");
    return instanceLayers;
  }
  
  void Renderer::initVulkan(Window& window, bool debug) { 
    this->instance = std::make_unique<VulkanInstance>(window.getTitle(), this->instanceExtensions, this->instanceLayers);
    this->surface = std::make_unique<VulkanSurface>(*this->instance, window.getHandle());
    this->physicalDevice = std::make_unique<VulkanPhysicalDevice>(*this->instance, getPhysicalDeviceRequirements);

    std::vector<const char*> enabledDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    this->device = std::make_unique<VulkanDevice>(*this->physicalDevice, *this->surface, enabledDeviceExtensions); 
    this->memoryAllocator = std::make_unique<VulkanMemoryAllocator>(*instance, *physicalDevice, *device);
    this->swapchain = std::make_unique<VulkanSwapchain>(
        *this->physicalDevice, 
        *this->device, 
        *this->surface, 
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        0
    );

    this->graphicsQueue = std::make_unique<VulkanQueue>(*this->device, this->device->getGraphicsQueueFamily(), 0);
    this->presentQueue = std::make_unique<VulkanQueue>(*this->device, this->device->getPresentQueueFamily(), 0);
    
    this->commandPool = std::make_unique<VulkanCommandPool>(*device, device->getGraphicsQueueFamily());
    this->commandBuffers = this->commandPool->allocateCommandBuffers(this->swapchain->getImageCount(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    uint32_t imageCount = this->swapchain->getImageCount();
    this->fences.reserve(imageCount);
    this->imageReadySemaphores.reserve(imageCount);
    this->frameFinishedSemaphores.reserve(imageCount);

    for (int i = 0; i < this->fences.capacity(); i++) {
      this->fences.emplace_back(*this->device, VK_FENCE_CREATE_SIGNALED_BIT);
      this->imageReadySemaphores.emplace_back(*this->device, 0);
      this->frameFinishedSemaphores.emplace_back(*this->device, 0);
    }
  }

  bool Renderer::getPhysicalDeviceRequirements(VkPhysicalDevice& physicalDevice) {
    return true;
  }
}
