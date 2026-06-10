#include "radiant/core/engine/RadiantEngine.h"
#include "radiant/core/engine/layout/WidgetManager.h"
#include "radiant/core/render/models/Quad2D.h"
#include "radiant/core/render/Window.h"
#include <memory>
#include <string>

namespace Radiant {
  RadiantEngine::RadiantEngine(const std::string& title, uint32_t width, uint32_t height) {
    this->window = std::make_unique<Window>(title, width, height);
    this->renderer = std::make_unique<Renderer>(*this->window, true);
    this->widgetManager = std::make_unique<WidgetManager>(*this->window);

    this->vertexBuffer = renderer->createVertexBuffer(2048);
    this->instanceBuffer = renderer->createInstanceBuffer(2048);
    this->indexBuffer = renderer->createIndexBuffer(2048);


    Quad2D quad;
    std::vector<Vertex> verticies = quad.getVerticies();
    std::vector<uint16_t> indicies = quad.getIndicies();
    //std::vector<Instance> instances = {
    //  {{255, 0, 0, 255}, {100, 100, 0}, {200, 200}}
    //};

    this->vertexBuffer->append(verticies);
    this->indexBuffer->append(indicies);
  }

  RadiantEngine::~RadiantEngine() {
    renderer->waitIdle();
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
