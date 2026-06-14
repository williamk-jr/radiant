#pragma once

#include "radiant/core/engine/layout/WidgetManager.h"
#include "radiant/core/render/Renderer.h"
#include "radiant/core/render/Window.h"
#include "radiant/css/css_parser.h"
#include <cstdint>
#include <memory>
namespace Radiant {
  class RadiantEngine {
    public:
      RadiantEngine(const std::string& title, uint32_t width, uint32_t height);
      ~RadiantEngine();

      std::shared_ptr<Widget> getRootWidget();
      void loadStylesheet(std::filesystem::path path);
      bool isRunning();
      void update();
    private:
      std::unique_ptr<Window> window;
      std::unique_ptr<WidgetManager> widgetManager;

      std::unique_ptr<Renderer> renderer;
      std::unique_ptr<VertexBuffer> vertexBuffer;
      std::unique_ptr<InstanceBuffer> instanceBuffer;
      std::unique_ptr<IndexBuffer> indexBuffer;

      std::unique_ptr<CssParser> stylesheetParser;
  };
}
