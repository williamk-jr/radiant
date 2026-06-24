#include "radiant/core/engine/RadiantEngine.h"
#include "radiant/css/StyleSheetValue.h"
#include "radiant/css/values/Color.h"
#include "radiant/css/values/Unit.h"
#include "radiant/core/engine/widgets/widget.h"
#include "radiant/core/render/Rect2D.h"
#include <cstdint>
#include <string>
#define GLFW_INCLUDE_VULKAN

#include <iostream>
#include <vector>

#include <vulkan/vk_platform.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vma/vk_mem_alloc.h>

#include <GLFW/glfw3.h>

#include "radiant/css/ast/abstract_syntax_tree.h"
#include "radiant/css/Parser.h"
#include "radiant/core/render/Renderer.h"
#include "radiant/core/render/Window.h"

#include "radiant/util/logger/Logger.h"
bool physicalDeviceRequirements(VkPhysicalDevice& physicalDevice) {
  return true;
}

int main() {
  Radiant::Logger::shouldAllowColor(true);
  Radiant::Logger::shouldAllowExceptions(false);
  Radiant::Logger::setVerbosity(1);

  const std::string applicationName = "Test";
  Radiant::RadiantEngine engine(applicationName, 500, 500);

  Radiant::Widget testWidget(
      engine.getRootWidget(),
      0, 0,
      200, 200
  );

  testWidget.addStyle("left", {
      {Radiant::StyleSheetParser::Unit(10.0, Radiant::StyleSheetParser::UnitType::PIXEL)}
  });
  testWidget.addStyle("top", {
      {Radiant::StyleSheetParser::Unit(10.0, Radiant::StyleSheetParser::UnitType::PIXEL)}
  });

  testWidget.addStyle("background-color", {
      {Radiant::StyleSheetParser::Color(0, 255, 0, 255)}
  });

  while (engine.isRunning()) {
    engine.update(); 
  }

  // Create Instance
  //std::vector<const char*> enabledExtensionNames = {VK_KHR_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
  //std::vector<const char*> enabledLayerNames = {"VK_LAYER_KHRONOS_validation"};

  //GLFWwindow* window;
  //glfwInit();
  //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  //window = glfwCreateWindow(500, 500, "Render test", NULL, NULL);

  //// Add glfw extensions to instance extensions.
  //uint32_t glfwExtensionCount;
  //const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  //for (int i = 0; i < glfwExtensionCount; i++) {
  //  enabledExtensionNames.push_back(glfwExtensions[i]);
  //}

  //Radiant::VulkanInstance instance(applicationName, enabledExtensionNames, enabledLayerNames);
  //Radiant::VulkanSurface surface(instance, window);
  //Radiant::VulkanPhysicalDevice physicalDevice(instance, physicalDeviceRequirements);

  //VkPhysicalDeviceProperties2 deviceProperties = {};
  //deviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
  //physicalDevice.getProperties(&deviceProperties);
  //std::cout << "Selected Device: " << deviceProperties.properties.deviceName << "\n";

  //std::vector<const char*> enabledDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  //Radiant::VulkanDevice device(physicalDevice, surface, enabledDeviceExtensions); 

  //glfwMakeContextCurrent(window);

  //while (!glfwWindowShouldClose(window)) {
  //  glfwSwapBuffers(window);
  //  glfwPollEvents();
  //}

  //glfwTerminate();
  //CssParser parser;

  //std::vector<Token> tokens = parser.tokenize("./assets/test.css");
  //for (Token token : tokens) {
  //  std::cout << CssParser::tokenTypeToString(token.getType()) << ": " << token.getValue() << "\n";
  //}

  //AbstractSyntaxTree ast(tokens);
  //ast.display();
  
  //std::string str1 = "347827347";
  //std::string str2 = "34.7827347";
  //std::string str3 = "34.7827.347";
  //std::string str4 = "347827347h";

  //std::cout << string_util::isNumeric(str1) << "\n";
  //std::cout << string_util::isNumeric(str2) << "\n";
  //std::cout << string_util::isNumeric(str3) << "\n";
  //std::cout << string_util::isNumeric(str4) << "\n";
}
