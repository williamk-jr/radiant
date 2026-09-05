cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(
  Radiant
  VERSION 1.0
  LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS
    ON
    CACHE INTERNAL "") # Required by clangd

# Find the GLM package
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(slang REQUIRED)
find_package(glm REQUIRED)
find_package(freetype REQUIRED)

if(glfw3_FOUND)
  add_definitions(-DHAS_GLFW)
elseif(SDL3_FOUND)
  add_definitions(-DHAS_SDL3)
endif()

add_subdirectory(src)

# Enable debug mode
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Testing
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_subdirectory("libs/Catch2-3.16.0")
  add_subdirectory(tests)
endif()

# Lists all imported targets. Makes it easy to find newly added targets from
# find_package.
get_property(
  importTargets
  DIRECTORY ${CMAKE_SOURCE_DIR}
  PROPERTY IMPORTED_TARGETS)
list(JOIN importTargets "\n\t" formatedImportTargets)
message(STATUS "Imported Targets:\n\t${formatedImportTargets}")
