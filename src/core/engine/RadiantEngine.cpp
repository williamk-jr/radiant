#include "radiant/core/engine/RadiantEngine.h"
#include "radiant/core/engine/StyleSheetPropertyResolvers.h"
#include "radiant/core/engine/layout/WidgetManager.h"
#include "radiant/core/render/models/Quad2D.h"
#include "radiant/core/render/Window.h"
#include "radiant/css/StyleSheetEntry.h"
#include "radiant/css/css_parser.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace Radiant {
  long testFunc(uint32_t num) {
    return 4*num;
  }

  RadiantEngine::RadiantEngine(const std::string& title, uint32_t width, uint32_t height) {
    this->window = std::make_unique<Window>(title, width, height);
    this->renderer = std::make_unique<Renderer>(*this->window, true);

    this->stylesheetParser = std::make_unique<CssParser>();
    this->registerProperties();

    RegisteredFunction func = this->stylesheetParser->registerFunction("testFunc", &testFunc);
    func.runtimeFunction({{4}});
    
    this->widgetManager = std::make_unique<WidgetManager>(*this->window, *this->stylesheetParser);

    this->vertexBuffer = renderer->createVertexBuffer(2048);
    this->instanceBuffer = renderer->createInstanceBuffer(2048);
    this->indexBuffer = renderer->createIndexBuffer(2048);



    std::unordered_map<std::string, StyleSheet> styleSheets = this->stylesheetParser->getStyleSheets("./assets/test.css");

    for (std::pair<std::string, StyleSheet> entry : styleSheets) {
      Logger::info(entry.first);
    }


    Quad2D quad;
    std::vector<Vertex> verticies = quad.getVerticies();
    std::vector<uint16_t> indicies = quad.getIndicies();
    //std::vector<Instance> instances = {
    //  {{255, 0, 0, 255}, {100, 100, 0}, {200, 200}}
    //};

    this->vertexBuffer->append(verticies);
    this->indexBuffer->append(indicies);
  }
  
  void RadiantEngine::registerProperties() {
    this->stylesheetParser->registerProperty("top", {StyleSheetValueTypes::UNIT}, {{Unit{0.0f, UnitType::PIXEL}}}, PropertyResolvers::UNIT_LIST_RESOLVER);
    this->stylesheetParser->registerProperty("bottom", {StyleSheetValueTypes::UNIT}, {{Unit{0.0f, UnitType::PIXEL}}}, PropertyResolvers::UNIT_LIST_RESOLVER);
    this->stylesheetParser->registerProperty("left", {StyleSheetValueTypes::UNIT}, {{Unit{0.0f, UnitType::PIXEL}}}, PropertyResolvers::UNIT_LIST_RESOLVER);
    this->stylesheetParser->registerProperty("right", {StyleSheetValueTypes::UNIT}, {{Unit{0.0f, UnitType::PIXEL}}}, PropertyResolvers::UNIT_LIST_RESOLVER);
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
    //Logger::info(std::to_string(batch.instances.size()));
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

    renderer->bindVertexBuffer(*this->vertexBuffer);
    renderer->bindInstanceBuffer(*this->instanceBuffer, sizeof(Instance)*batch.instances.size());
    renderer->bindIndexBuffer(*this->indexBuffer);

    renderer->drawIndexed(6, batch.instances.size());

    renderer->endRendering();
    renderer->endFrame();

    renderer->submit();
    renderer->present(*window);

    window->pollEvents();
  }
}
