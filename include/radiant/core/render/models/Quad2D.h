#pragma once

#include "radiant/core/render/Vertex.h"
#include "radiant/core/render/models/Model.h"
#include "radiant/util/Box.h"

#include <cstdint>
#include <vector>

namespace Radiant {
	class Quad2D : public Model {
		public:
			Quad2D();
			Quad2D(Box uv);
	};
} // namespace Radiant
