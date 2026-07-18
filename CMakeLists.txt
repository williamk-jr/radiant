cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(Radiant VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # Required by clangd

# Find the GLM package
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(slang REQUIRED)
find_package(glm REQUIRED)
find_package(freetype REQUIRED)

# Lists all imported targets. Makes it easy to find newly added targets from find_package.
get_property(importTargets DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY IMPORTED_TARGETS)
list(JOIN importTargets "\n\t" formatedImportTargets)
message(STATUS "Imported Targets:\n\t${formatedImportTargets}")

if (glfw3_FOUND)
  add_definitions(-DHAS_GLFW)
elseif (SDL3_FOUND)
  add_definitions(-DHAS_SDL3)
endif()

# Add the executable
add_executable(Radiant 
  src/main.cpp 

  src/css/Parser.cpp
  src/css/Token.cpp
  src/css/StyleSheetValue.cpp
  src/css/StyleSheetEntry.cpp
  src/css/StyleSheet.cpp
  src/css/values/Unit.cpp
  src/css/values/Integer.cpp
  src/css/values/Float.cpp
  src/css/values/Color.cpp
  src/css/values/Function.cpp
  src/css/ast/abstract_syntax_tree.cpp

  src/util/string_util.cpp
  src/util/logger/Logger.cpp

  src/core/engine/RadiantEngine.cpp
  src/core/engine/LayoutBox.cpp
  src/core/engine/StyleSheetPropertyResolvers.cpp
  src/core/engine/StyleSheetStandardFunctions.cpp
  src/core/engine/layout/WidgetManager.cpp
  src/core/engine/layout/LayoutManager.cpp
  src/core/engine/widgets/widget.cpp

  src/core/engine/font/Bitmap.cpp
  src/core/engine/font/Font.cpp
  src/core/engine/font/cache/FontCache.cpp
  src/core/engine/font/cache/FontGPUCache.cpp
  src/core/engine/font/FontManager.cpp

  src/core/render/vulkan/VulkanInstance.cpp
  src/core/render/vulkan/VulkanPhysicalDevice.cpp
  src/core/render/vulkan/VulkanDevice.cpp
  src/core/render/vulkan/VulkanSurface.cpp
  src/core/render/vulkan/VulkanCommandPool.cpp
  src/core/render/vulkan/VulkanCommandBuffer.cpp
  src/core/render/vulkan/VulkanMemoryAllocator.cpp
  src/core/render/vulkan/VulkanQueue.cpp
  src/core/render/vulkan/VulkanFence.cpp
  src/core/render/vulkan/VulkanSemaphore.cpp
  src/core/render/vulkan/VulkanBinarySemaphore.cpp
  src/core/render/vulkan/VulkanTimelineSemaphore.cpp
  src/core/render/vulkan/VulkanSwapchain.cpp
  src/core/render/vulkan/VulkanImage.cpp
  src/core/render/vulkan/VulkanImageView.cpp
  src/core/render/vulkan/VulkanGraphicsPipelineBuilder.cpp
  src/core/render/vulkan/VulkanPipeline.cpp
  src/core/render/vulkan/VulkanDescriptorSetLayout.cpp
  src/core/render/vulkan/VulkanDescriptorPool.cpp
  src/core/render/vulkan/VulkanDescriptorSet.cpp
  src/core/render/vulkan/VulkanBuffer.cpp
  src/core/render/vulkan/VulkanUtil.cpp

  src/core/render/Color.cpp
  src/core/render/Renderer.cpp
  src/core/render/Window.cpp
  src/core/render/TextureAtlas.cpp
  src/core/render/models/Model.cpp
  src/core/render/models/Quad2D.cpp
  src/core/render/buffers/VertexBuffer.cpp
  src/core/render/buffers/InstanceBuffer.cpp
  src/core/render/buffers/IndexBuffer.cpp
)

target_link_libraries(Radiant 
  PRIVATE Vulkan::Vulkan
  PRIVATE Freetype::Freetype
  PRIVATE slang::slang
  PRIVATE glm::glm
  PRIVATE glfw
)

target_include_directories(Radiant 
  PUBLIC ${CMAKE_SOURCE_DIR}/include
  PRIVATE ${Vulkan_INCLUDE_DIRS}  
)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

