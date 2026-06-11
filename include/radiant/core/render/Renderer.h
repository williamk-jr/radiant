#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "radiant/core/render/Color.h"
#include "radiant/core/render/buffers/IndexBuffer.h"
#include "radiant/core/render/Rect2D.h"
#include "radiant/core/render/buffers/InstanceBuffer.h"
#include "radiant/core/render/buffers/VertexBuffer.h"
#include "radiant/core/render/Window.h"
#include "radiant/core/render/vulkan/VulkanBinarySemaphore.h"
#include "radiant/core/render/vulkan/VulkanDescriptorPool.h"
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanFence.h"
#include "radiant/core/render/vulkan/VulkanGraphicsPipelineBuilder.h"
#include "radiant/core/render/vulkan/VulkanImageView.h"
#include "radiant/core/render/vulkan/VulkanInstance.h"
#include "radiant/core/render/vulkan/VulkanMemoryAllocator.h"
#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"
#include "radiant/core/render/vulkan/VulkanPipeline.h"
#include "radiant/core/render/vulkan/VulkanQueue.h"
#include "radiant/core/render/vulkan/VulkanSurface.h"
#include "radiant/core/render/vulkan/VulkanSwapchain.h"
#include "radiant/core/render/vulkan/VulkanCommandPool.h"
#include "radiant/core/render/vulkan/VulkanImage.h"

namespace Radiant {
  struct RenderContext {
    VulkanImageView& imageView;
    uint32_t imageIndex;
    bool rendering = false;

    RenderContext(VulkanImageView& imageView, uint32_t imageIndex, bool rendering) :
      imageView(imageView), imageIndex(imageIndex), rendering(rendering) {}
  };


  class Renderer {
    public:
      Renderer(Window& window, bool debug);
      void waitIdle();

      std::unique_ptr<VertexBuffer> createVertexBuffer(VkDeviceSize size);
      std::unique_ptr<IndexBuffer> createIndexBuffer(VkDeviceSize size);
      std::unique_ptr<InstanceBuffer> createInstanceBuffer(VkDeviceSize size);

      void beginFrame(Window& window);
      void beginRendering(Color clearColor);
      void setViewport(float width, float height, float minDepth, float maxDepth);
      void setScissor(uint32_t width, uint32_t height);

      void bindVertexBuffer(VertexBuffer& vertexBuffer);
      void bindVertexBuffer(VertexBuffer& vertexBuffer, VkDeviceSize size);
      void bindInstanceBuffer(InstanceBuffer& instanceBuffer);
      void bindInstanceBuffer(InstanceBuffer& instanceBuffer, VkDeviceSize size);
      void bindIndexBuffer(IndexBuffer& indexBuffer);

      void drawIndexed(uint32_t indexCount, uint32_t instanceCount);
      void clear(Color color);
      void clear(Color color, VkRect2D clearArea);
      void endRendering();
      void endFrame();

      void submit();
      void present(Window& window);
      
    private:
      std::vector<const char*> instanceExtensions;
      std::vector<const char*> instanceLayers;

      std::unique_ptr<VulkanInstance> instance;
      std::unique_ptr<VulkanSurface> surface;
      std::unique_ptr<VulkanPhysicalDevice> physicalDevice;

      std::unique_ptr<VulkanDevice> device;
      std::unique_ptr<VulkanMemoryAllocator> memoryAllocator;
      std::unique_ptr<VulkanQueue> graphicsQueue;
      std::unique_ptr<VulkanQueue> presentQueue;
      std::unique_ptr<VulkanSwapchain> swapchain;

      std::unique_ptr<VulkanCommandPool> commandPool;
      std::vector<VulkanCommandBuffer> commandBuffers;
      
      std::vector<VulkanFence> fences;
      std::vector<VulkanBinarySemaphore> imageReadySemaphores;
      std::vector<VulkanBinarySemaphore> frameFinishedSemaphores;

      std::unique_ptr<VulkanDescriptorPool> descriptorPool;
      std::vector<VulkanDescriptorSetLayout> descriptorSetLayouts;
      std::vector<VulkanDescriptorSet> descriptorSets;
      std::unique_ptr<VulkanPipeline> graphicsPipeline;

      std::unique_ptr<RenderContext> context;
      int currentFrame = 0;
      bool updateSwapchain = false;
      Rect2D frameBufferSize;
      
      void initVulkan(Window& window, bool debug);
      std::vector<const char*> getInstanceExtensions(Window& window, bool debug);
      std::vector<const char*> getInstanceLayers(bool debug);
      static bool getPhysicalDeviceRequirements(VkPhysicalDevice& physicalDevice);
  };
}
