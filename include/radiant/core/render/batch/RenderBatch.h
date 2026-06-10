#pragma once

#include "radiant/core/render/Instance.h"
#include "radiant/core/render/models/Model.h"
#include <vector>
namespace Radiant {
  struct RenderBatch {
    Model model;
    std::vector<Instance> instances;
  };
}
