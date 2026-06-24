#include "radiant/core/render/Renderer.h"
#include "radiant/core/render/Instance.h"
#include "radiant/core/render/Rect2D.h"
#include "radiant/core/render/Vertex.h"
#include "radiant/core/render/vulkan/VulkanDescriptorPool.h"
#include "radiant/core/render/vulkan/VulkanDescriptorSetLayout.h"
#include "radiant/core/render/vulkan/VulkanGraphicsPipelineBuilder.h"
#include <cstddef>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Radiant {
  Renderer::Renderer(Window& window, bool debug) {
    this->instanceExtensions = this->getInstanceExtensions(window, debug); 
    this->instanceLayers = this->getInstanceLayers(debug);
    this->frameBufferSize = window.getFrameBufferSize();

    this->initVulkan(window, debug);
  }

  void Renderer::waitIdle() {
    this->device->waitIdle();
  } 

  std::unique_ptr<VertexBuffer> Renderer::createVertexBuffer(VkDeviceSize size) {
    return std::make_unique<VertexBuffer>(*this->memoryAllocator, size);
  }

  std::unique_ptr<IndexBuffer> Renderer::createIndexBuffer(VkDeviceSize size) {
    return std::make_unique<IndexBuffer>(*this->memoryAllocator, size);
  }

  std::unique_ptr<InstanceBuffer> Renderer::createInstanceBuffer(VkDeviceSize size) {
    return std::make_unique<InstanceBuffer>(*this->memoryAllocator, size);
  }

  void Renderer::beginFrame(Window& window) {
    this->fences[currentFrame].wait(UINT32_MAX);
    this->fences[currentFrame].reset();

    VulkanResult<uint32_t> imageIndex = this->swapchain->acquireNextImage(&this->imageReadySemaphores[currentFrame], UINT64_MAX);

    Rect2D newFrameBufferSize = window.getFrameBufferSize();
    if (
        (imageIndex.result == VK_ERROR_OUT_OF_DATE_KHR || imageIndex.result == VK_SUBOPTIMAL_KHR) && 
        (newFrameBufferSize.width != this->frameBufferSize.width) || (newFrameBufferSize.height != this->frameBufferSize.height)
    ) {
      this->updateSwapchain = true;
    }

    VulkanImage& currentImage = this->swapchain->getImage(imageIndex.value);
    VulkanImageView& currentImageView = this->swapchain->getImageView(imageIndex.value);

    VkImageSubresourceRange subresourceRange{};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;

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
      imageIndex.value,
      true
    );
  }

  void Renderer::beginRendering(Color clearColor) {
    if (!this->context->rendering) {
      return;
    }

    float* rawColor = clearColor.raw();

    std::vector<VkRenderingAttachmentInfo> colorAttachment(1);
    colorAttachment[0].sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    colorAttachment[0].imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
    colorAttachment[0].imageView = this->context->imageView.get(); 
    colorAttachment[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment[0].clearValue = {{rawColor[0], rawColor[1], rawColor[2], rawColor[3]}};

    VkRect2D renderArea{};
    renderArea.extent = {this->frameBufferSize.width, this->frameBufferSize.height};

    this->commandBuffers[currentFrame].beginRendering(&colorAttachment, nullptr, nullptr, renderArea, 0);
    this->commandBuffers[currentFrame].bindPipeline(*this->graphicsPipeline);

    // Update uniform buffer
    this->descriptorBuffer->resetOffset();
    //uint32_t frameBufferUniformData[2] = {this->frameBufferSize.width, this->frameBufferSize.height};
    float aspect = (float)this->frameBufferSize.width / (float) this->frameBufferSize.height;
    glm::mat4 orthoMatrix = glm::ortho(0.0f, (float)this->frameBufferSize.width, 0.0f, (float)this->frameBufferSize.height, -1.0f, 1.0f);
    //glm::mat4 orthoMatrix = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
    this->descriptorBuffer->append(&orthoMatrix, sizeof(glm::mat4));
    
    // Update uniforms
    this->descriptorPool->updateDescriptorSets({
      VulkanWriteDescriptorSet{this->descriptorSets[currentFrame].get(), 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, {
        VkDescriptorBufferInfo{this->descriptorBuffer->get(), 0, sizeof(glm::mat4)}
      }, {}, {}}
    }, {});
    this->commandBuffers[currentFrame].bindDescriptorSets(*this->graphicsPipeline, 0, this->descriptorSets);
  }

  void Renderer::setViewport(float width, float height, float minDepth, float maxDepth) {
    if (!this->context->rendering) {
      return;
    }
    this->commandBuffers[currentFrame].setViewport(width, height, minDepth, maxDepth);
  }

  void Renderer::setScissor(uint32_t width, uint32_t height) {
    if (!this->context->rendering) {
      return;
    }
    this->commandBuffers[currentFrame].setScissor(width, height);
  }


  void Renderer::bindVertexBuffer(VertexBuffer& vertexBuffer) {
    this->commandBuffers[currentFrame].bindVertexBuffer(vertexBuffer.get(), 0, 0);
  }

  void Renderer::bindVertexBuffer(VertexBuffer& vertexBuffer, VkDeviceSize size) {
    this->commandBuffers[currentFrame].bindVertexBuffer(vertexBuffer.get(), 0, 0, size);
  }

  void Renderer::bindInstanceBuffer(InstanceBuffer& instanceBuffer) {
    this->commandBuffers[currentFrame].bindVertexBuffer(instanceBuffer.get(), 1, 0);
  }

  void Renderer::bindInstanceBuffer(InstanceBuffer& instanceBuffer, VkDeviceSize size) {
    this->commandBuffers[currentFrame].bindVertexBuffer(instanceBuffer.get(), 1, 0, size);
  }

  void Renderer::bindIndexBuffer(IndexBuffer& indexBuffer) {
    this->commandBuffers[currentFrame].bindIndexBuffer(indexBuffer.get(), 0, VK_INDEX_TYPE_UINT16);
  }


  void Renderer::drawIndexed(uint32_t indexCount, uint32_t instanceCount) {
    this->commandBuffers[currentFrame].drawIndexed(indexCount, instanceCount);
  }

  void Renderer::clear(Color color) {
    if (!this->context->rendering) {
      return;
    }
    VkExtent2D swapchainExtent = {this->frameBufferSize.width, this->frameBufferSize.height};
    this->clear(color, {{0,0},swapchainExtent});
  }

  void Renderer::clear(Color color, VkRect2D clearArea) {
    if (!this->context->rendering) {
      return;
    }
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
    if (!this->context->rendering) {
      return;
    }
    this->commandBuffers[currentFrame].endRendering();
  }

  void Renderer::endFrame() {
    if (!this->context->rendering) {
      return;
    }
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
    this->context->rendering = false;
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

  void Renderer::present(Window& window) {
    this->presentQueue->present(*this->swapchain, {this->context->imageIndex}, this->frameFinishedSemaphores[currentFrame]);
    this->currentFrame = (currentFrame+1)%swapchain->getImageCount();

    if (this->updateSwapchain) {
      this->frameBufferSize = window.getFrameBufferSize();

      this->updateSwapchain = false;
      this->swapchain->recreate(
          *this->physicalDevice,
          *this->device, 
          *this->surface, 
          VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
          0
      );
      return;
    }
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

    this->descriptorBuffer = std::make_unique<VulkanBuffer>(
        *this->memoryAllocator, 2048, 
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
        VK_SHARING_MODE_EXCLUSIVE, std::vector<uint32_t>{}
    );
    
    for (int i = 0; i < this->swapchain->getImageCount(); i++) {
      this->descriptorSetLayouts.emplace_back(*this->device, std::vector<VkDescriptorSetLayoutBinding>{
        VkDescriptorSetLayoutBinding{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr}
      });
    }
    //VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT

    this->descriptorPool = std::make_unique<VulkanDescriptorPool>(*this->device, std::vector<VkDescriptorPoolSize>{
      {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 3}
    }, 10);

    this->descriptorSets = this->descriptorPool->allocateDescriptorSets(this->descriptorSetLayouts);


    VkPipelineColorBlendAttachmentState attachmentState{};
    attachmentState.colorWriteMask = 0xF;

    this->graphicsPipeline = std::make_unique<VulkanPipeline>(VulkanGraphicsPipelineBuilder(*this->device)
      .withLayout(this->descriptorSetLayouts)
      .withRenderingInfo({VK_FORMAT_B8G8R8A8_SRGB}, VK_FORMAT_UNDEFINED, VK_FORMAT_UNDEFINED)
      .withVertexBindingDescription(sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX, {
        {VK_FORMAT_R32G32B32_SFLOAT, 0},
        {VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)},
        {VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)},
      })
      .withVertexBindingDescription(sizeof(Instance), VK_VERTEX_INPUT_RATE_INSTANCE, {
        {VK_FORMAT_R32G32B32A32_SFLOAT, 0},
        {VK_FORMAT_R32G32B32_SFLOAT, offsetof(Instance, position)},
        {VK_FORMAT_R32G32_SFLOAT, offsetof(Instance, size)},
      })
      .withInputAssemblyState(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE)
      .withShaderSlang("main", "./shaders/main.slang", VK_SHADER_STAGE_VERTEX_BIT)
      .withMultisampleState(VK_SAMPLE_COUNT_1_BIT, {VK_FALSE, 0.0}, VK_FALSE, VK_FALSE)
      .withRasterizationState(
          VK_POLYGON_MODE_FILL, 
          VK_CULL_MODE_NONE, 
          VK_FRONT_FACE_COUNTER_CLOCKWISE, 
          {VK_FALSE}, 
          1.0, 
          VK_FALSE
      )
      .withShaderSlang("main", "./shaders/main.slang", VK_SHADER_STAGE_FRAGMENT_BIT)
      .withColorBlendState({attachmentState}, nullptr, VK_LOGIC_OP_COPY)
      .withDynamicState({VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR})
      .withViewportState(1, 1)
      .build()
    );
  }

  bool Renderer::getPhysicalDeviceRequirements(VkPhysicalDevice& physicalDevice) {
    return true;
  }
}
