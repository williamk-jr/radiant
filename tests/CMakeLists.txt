add_executable(
  RadiantTests
  unit_tests/core/render/vulkan/VulkanInstanceTest.cpp
  unit_tests/core/render/vulkan/VulkanPhysicalDeviceTest.cpp
  unit_tests/core/render/vulkan/VulkanTestUtil.cpp)

target_link_libraries(
  RadiantTests
  PRIVATE Radiant
  PRIVATE Catch2::Catch2WithMain)

target_include_directories(RadiantTests PUBLIC ${CMAKE_SOURCE_DIR}/include)

include(CTest)
include(Catch)

catch_discover_tests(RadiantTests)
