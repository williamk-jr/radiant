#include "radiant/core/render/Renderer.h"
#include "radiant/core/render/vulkan/VulkanCommandPool.h"
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanFence.h"
#include "radiant/core/render/vulkan/VulkanImage.h"
#include "radiant/core/render/vulkan/VulkanQueue.h"
#include "radiant/util/logger/Logger.h"
#include <memory>
#include <string>
#include <vulkan/vulkan_core.h>

namespace Radiant {
  Renderer::Renderer(Window& window, bool debug) {
    this->instanceExtensions = this->getInstanceExtensions(window, debug); 
    this->instanceLayers = this->getInstanceLayers(debug);

    this->initVulkan(window, debug);
  }
  void Renderer::waitIdle() {
    this->device->waitIdle();
  } 
  
  void Renderer::renderLoop() {
    // TODO layout transitions for images
    this->fences[currentFrame].wait(UINT32_MAX);

    uint32_t imageIndex = this->swapchain->acquireNextImage(&this->imageReadySemaphores[currentFrame], UINT64_MAX);
    //Logger::info(std::to_string(imageIndex));
    this->fences[currentFrame].reset();
    this->commandBuffers[currentFrame].reset(false);
    
    VkClearColorValue color{};
    color.uint32[0] = 0;
    color.uint32[1] = 100;
    color.uint32[2] = 0;
    color.uint32[3] = 100;

    this->commandBuffers[currentFrame].begin(0);
    this->commandBuffers[currentFrame].clearColor(
      this->swapchain->getImage(imageIndex),
      color
    );
    this->commandBuffers[currentFrame].end();

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

    std::vector<VkSemaphore> rawSemaphores{this->frameFinishedSemaphores[currentFrame].get()};

    this->presentQueue->present(*this->swapchain, {imageIndex}, rawSemaphores);
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

    VkPhysicalDeviceProperties2 deviceProperties = {};
    deviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    physicalDevice->getProperties(&deviceProperties);
    std::cout << "Selected Device: " << deviceProperties.properties.deviceName << "\n";

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
    Logger::info(std::to_string(this->frameFinishedSemaphores.size()));
  }

  bool Renderer::getPhysicalDeviceRequirements(VkPhysicalDevice& physicalDevice) {
    return true;
  }
}
