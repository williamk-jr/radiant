#include "radiant/core/engine/RadiantEngine.h"
#include "radiant/core/engine/StyleSheetPropertyResolvers.h"
#include "radiant/core/engine/StyleSheetStandardFunctions.h"
#include "radiant/core/engine/font/Bitmap.h"
#include "radiant/core/engine/font/Font.h"
#include "radiant/core/engine/layout/WidgetManager.h"
#include "radiant/core/render/models/Quad2D.h"
#include "radiant/core/render/Window.h"
#include "radiant/css/StyleSheetEntry.h"
#include "radiant/css/Parser.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Radiant {
  RadiantEngine::RadiantEngine(const std::string& title, uint32_t width, uint32_t height) {
    this->window = std::make_unique<Window>(title, width, height);
    this->fontManager = std::make_unique<FontManager>();
    Font notoSans = this->fontManager->loadFont("/usr/share/fonts/truetype/noto/NotoSans-Regular.ttf");

    notoSans.setPointSize(16);
    this->fontManager->compileStringGeometry(notoSans, "Hello World.");
    //Bitmap bitmap = notoSans.getBitmapFromCharCode('x');

    // Enable debug logs.
#ifndef NDEBUG
    this->renderer = std::make_unique<Renderer>(*this->window, true);
#else
    this->renderer = std::make_unique<Renderer>(*this->window, false);
#endif

    this->stylesheetParser = std::make_unique<StyleSheetParser::Parser>();
    this->registerProperties();
    this->registerFunctions();

    this->widgetManager = std::make_unique<WidgetManager>(*this->window, *this->stylesheetParser);

    this->vertexBuffer = renderer->createVertexBuffer(2048);
    this->instanceBuffer = renderer->createInstanceBuffer(2048);
    this->indexBuffer = renderer->createIndexBuffer(2048);

    std::unordered_map<std::string, StyleSheetParser::StyleSheet> styleSheets = this->stylesheetParser->getStyleSheets("./assets/test.css");

    Quad2D quad;
    std::vector<Vertex> verticies = quad.getVerticies();
    std::vector<uint16_t> indicies = quad.getIndicies();

    this->vertexBuffer->append(verticies);
    this->indexBuffer->append(indicies);
  }
  
  void RadiantEngine::registerProperties() {
    this->stylesheetParser->registerProperty("top", {StyleSheetParser::ValueTypes::UNIT}, {{StyleSheetParser::Unit{0.0f, StyleSheetParser::UnitType::PIXEL}}}, PropertyResolvers::UNIT_LIST_RESOLVER);
    this->stylesheetParser->registerProperty("bottom", {StyleSheetParser::ValueTypes::UNIT}, {{StyleSheetParser::Unit{0.0f, StyleSheetParser::UnitType::PIXEL}}}, PropertyResolvers::UNIT_LIST_RESOLVER);
    this->stylesheetParser->registerProperty("left", {StyleSheetParser::ValueTypes::UNIT}, {{StyleSheetParser::Unit{0.0f, StyleSheetParser::UnitType::PIXEL}}}, PropertyResolvers::UNIT_LIST_RESOLVER);
    this->stylesheetParser->registerProperty("right", {StyleSheetParser::ValueTypes::UNIT}, {{StyleSheetParser::Unit{0.0f, StyleSheetParser::UnitType::PIXEL}}}, PropertyResolvers::UNIT_LIST_RESOLVER);
  }

  void RadiantEngine::registerFunctions() {
    this->stylesheetParser->registerFunction("rgb", &StyleSheetStandardFunctions::rgb);
  }

  RadiantEngine::~RadiantEngine() {
    renderer->waitIdle();
  }
  
  void RadiantEngine::loadStylesheet(std::filesystem::path path) {
     
  }

  std::shared_ptr<Widget> RadiantEngine::getRootWidget() {
    return this->widgetManager->getRootWidget();
  }
  
  bool RadiantEngine::isRunning() {
    return !this->window->shouldClose();
  }

  void RadiantEngine::update() {
    RenderBatch batch = this->widgetManager->createRenderBatch();
    if (!batch.instances.empty()) { // If empty, instanceBuffer does not need to be updated
      this->instanceBuffer->resetOffset();
      this->instanceBuffer->append(batch.instances);
    }

    Radiant::Color color{0,0,0,1};
    renderer->beginFrame(*window);
    renderer->beginRendering(color);

    Radiant::Rect2D frameBufferSize = window->getFrameBufferSize();
    renderer->setViewport(frameBufferSize.width, frameBufferSize.height, 0, 1.0);
    renderer->setScissor(frameBufferSize.width, frameBufferSize.height);

    // Update uniforms
    glm::mat4 orthoMatrix = glm::ortho(0.0f, (float)frameBufferSize.width, 0.0f, (float)frameBufferSize.height, -1.0f, 1.0f);
    renderer->updateUniformBuffer(orthoMatrix);
    renderer->bindDescriptorSets();

    // Bind buffers
    renderer->bindVertexBuffer(*this->vertexBuffer);
    renderer->bindInstanceBuffer(*this->instanceBuffer, sizeof(Instance)*batch.instances.size());
    renderer->bindIndexBuffer(*this->indexBuffer);

    // Draw
    renderer->drawIndexed(6, batch.instances.size());

    renderer->endRendering();
    renderer->endFrame();

    renderer->submit();
    renderer->present(*window);

    window->pollEvents();
  }
}
