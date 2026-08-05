#pragma once
#include "radiant/core/render/Color.h"
#include "radiant/core/render/Rect2D.h"
#include "radiant/core/render/Window.h"
#include "radiant/core/render/resources/IndexBuffer.h"
#include "radiant/core/render/resources/InstanceBuffer.h"
#include "radiant/core/render/resources/VertexBuffer.h"
#include "radiant/core/render/resources/shader/ShaderResourceManager.h"
#include "radiant/core/render/resources/shader/Texture.h"
#include "radiant/core/render/resources/shader/UniformBuffer.h"
#include "radiant/core/render/vulkan/VulkanCommandPool.h"
#include "radiant/core/render/vulkan/VulkanDevice.h"
#include "radiant/core/render/vulkan/VulkanInstance.h"
#include "radiant/core/render/vulkan/VulkanMemoryAllocator.h"
#include "radiant/core/render/vulkan/VulkanPhysicalDevice.h"
#include "radiant/core/render/vulkan/VulkanQueue.h"
#include "radiant/core/render/vulkan/VulkanSurface.h"
#include "radiant/core/render/vulkan/VulkanSwapchain.h"
#include "radiant/core/render/vulkan/descriptor/VulkanDescriptorPool.h"
#include "radiant/core/render/vulkan/pipeline/VulkanPipeline.h"
#include "radiant/core/render/vulkan/syncronization/VulkanBinarySemaphore.h"
#include "radiant/core/render/vulkan/syncronization/VulkanFence.h"

#include <cstdint>
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Radiant {
	struct RenderContext {
			uint32_t imageIndex;
			bool     rendering = false;
	};

	class Renderer {
		public:
			Renderer(Window& window, bool debug);

			void waitIdle();

			std::unique_ptr<VertexBuffer> createVertexBuffer(VkDeviceSize size);

			std::unique_ptr<IndexBuffer> createIndexBuffer(VkDeviceSize size);

			std::unique_ptr<InstanceBuffer> createInstanceBuffer(VkDeviceSize size);

			std::unique_ptr<UniformBuffer> createUniformBuffer(VkDeviceSize size);

			std::unique_ptr<Texture> createTexture(void* buffer, uint32_t width, uint32_t height, uint32_t pixelSize);

			void beginFrame(Window& window);

			void beginRendering(Color clearColor);

			void setViewport(float width, float height, float minDepth, float maxDepth);

			void setScissor(uint32_t width, uint32_t height);

			void bindPipeline(VulkanPipeline& pipeline);

			void bindResource(VulkanPipeline& pipeline, ShaderResource& resource, uint32_t firstSet);

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

			VulkanDevice& getDevice();

			std::vector<VkDescriptorSetLayout> getDescriptorSetLayouts();

		private:
			std::vector<const char*> instanceExtensions;
			std::vector<const char*> instanceLayers;

			std::unique_ptr<VulkanInstance>       instance;
			std::unique_ptr<VulkanSurface>        surface;
			std::unique_ptr<VulkanPhysicalDevice> physicalDevice;

			std::unique_ptr<VulkanDevice>          device;
			std::unique_ptr<VulkanMemoryAllocator> memoryAllocator;
			std::unique_ptr<VulkanQueue>           graphicsQueue;
			std::unique_ptr<VulkanQueue>           presentQueue;
			std::unique_ptr<VulkanSwapchain>       swapchain;

			std::unique_ptr<VulkanCommandPool> commandPool;
			std::vector<VulkanCommandBuffer>   commandBuffers;

			std::vector<VulkanFence>           fences;
			std::vector<VulkanBinarySemaphore> imageReadySemaphores;
			std::vector<VulkanBinarySemaphore> frameFinishedSemaphores;

			std::unique_ptr<ShaderResourceManager> shaderResourceManager;

			// std::unique_ptr<VulkanBuffer>         descriptorBuffer;
			std::unique_ptr<VulkanDescriptorPool> descriptorPool;

			std::unique_ptr<VulkanDescriptorSetLayout> frameDescriptorSetLayout;
			std::unique_ptr<VulkanDescriptorSetLayout> textureDescriptorSetLayout;

			std::vector<VulkanDescriptorSet> descriptorSets;

			RenderContext context;

			int    currentFrame    = 0;
			bool   updateSwapchain = false;
			Rect2D frameBufferSize;

			void initVulkan(Window& window, bool debug);

			void initShaderResourceManager();

			void initGraphicsPipeline();

			bool isSwapchainOutOfDate(Window& window, VulkanResult<uint32_t> imageIndex);

			std::vector<const char*> getInstanceExtensions(Window& window, bool debug);

			std::vector<const char*> getInstanceLayers(bool debug);

			static bool getPhysicalDeviceRequirements(VkPhysicalDevice& physicalDevice);
	};
} // namespace Radiant
