add_library(
  Radiant STATIC
  core/engine/font/Bitmap.cpp
  core/engine/font/cache/FontCache.cpp
  core/engine/font/cache/FontGPUCache.cpp
  core/engine/font/Font.cpp
  core/engine/font/FontManager.cpp
  core/engine/layout/LayoutManager.cpp
  core/engine/layout/WidgetManager.cpp
  core/engine/LayoutBox.cpp
  core/engine/RadiantEngine.cpp
  core/engine/StyleSheetPropertyResolvers.cpp
  core/engine/StyleSheetStandardFunctions.cpp
  core/engine/widgets/widget.cpp
  core/render/Color.cpp
  core/render/models/Model.cpp
  core/render/models/Quad2D.cpp
  core/render/Renderer.cpp
  core/render/resources/IndexBuffer.cpp
  core/render/resources/InstanceBuffer.cpp
  core/render/resources/shader/ShaderResource.cpp
  core/render/resources/shader/ShaderResourceManager.cpp
  core/render/resources/shader/Texture.cpp
  core/render/resources/shader/UniformBuffer.cpp
  core/render/resources/VertexBuffer.cpp
  core/render/TextureAtlas.cpp
  core/render/vulkan/descriptor/VulkanDescriptorPool.cpp
  core/render/vulkan/descriptor/VulkanDescriptorSet.cpp
  core/render/vulkan/descriptor/VulkanDescriptorSetLayout.cpp
  core/render/vulkan/pipeline/VulkanGraphicsPipelineBuilder.cpp
  core/render/vulkan/pipeline/VulkanPipeline.cpp
  core/render/vulkan/resource/VulkanBuffer.cpp
  core/render/vulkan/resource/VulkanImage.cpp
  core/render/vulkan/resource/VulkanImageView.cpp
  core/render/vulkan/syncronization/VulkanBinarySemaphore.cpp
  core/render/vulkan/syncronization/VulkanFence.cpp
  core/render/vulkan/syncronization/VulkanSemaphore.cpp
  core/render/vulkan/syncronization/VulkanTimelineSemaphore.cpp
  core/render/vulkan/VulkanCommandBuffer.cpp
  core/render/vulkan/VulkanCommandPool.cpp
  core/render/vulkan/VulkanDevice.cpp
  core/render/vulkan/VulkanInstance.cpp
  core/render/vulkan/VulkanMemoryAllocator.cpp
  core/render/vulkan/VulkanPhysicalDevice.cpp
  core/render/vulkan/VulkanQueue.cpp
  core/render/vulkan/VulkanSampler.cpp
  core/render/vulkan/VulkanSurface.cpp
  core/render/vulkan/VulkanSwapchain.cpp
  core/render/vulkan/VulkanUtil.cpp
  core/render/Window.cpp
  css/ast/abstract_syntax_tree.cpp
  css/Parser.cpp
  css/StyleSheet.cpp
  css/StyleSheetEntry.cpp
  css/StyleSheetValue.cpp
  css/Token.cpp
  css/values/Color.cpp
  css/values/Float.cpp
  css/values/Function.cpp
  css/values/Integer.cpp
  css/values/Unit.cpp
  util/Box.cpp
  util/debug/ExecutionProfiler.cpp
  util/logger/Logger.cpp
  util/string_util.cpp)

target_link_libraries(
  Radiant
  PRIVATE Vulkan::Vulkan
  PRIVATE Freetype::Freetype
  PRIVATE slang::slang
  PRIVATE glm::glm
  PRIVATE glfw)

target_include_directories(
  Radiant
  PUBLIC ${CMAKE_SOURCE_DIR}/include
  PUBLIC ${Vulkan_INCLUDE_DIRS})

add_executable(RadiantDemo main.cpp)
target_link_libraries(
  RadiantDemo
  PRIVATE Radiant
  PRIVATE Freetype::Freetype)

target_include_directories(RadiantDemo PUBLIC ${CMAKE_SOURCE_DIR}/include)
