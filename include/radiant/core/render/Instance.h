#pragma once
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

namespace Radiant {
  struct Instance {
    glm::vec4 color;
    glm::vec2 position;
    glm::vec2 size;
    
    glm::vec4 borderColor;
    glm::vec2 borderPosition;
    glm::vec2 borderSize;
  };
}
